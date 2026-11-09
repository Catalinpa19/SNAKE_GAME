#include "app/Catalog.h"

#include <QCoreApplication>

#include <algorithm>

namespace
{
constexpr ColorSource kFree = ColorSource::Default;
constexpr ColorSource kAchievement = ColorSource::Achievement;
constexpr ColorSource kShop = ColorSource::Shop;

constexpr std::array kSnakeColors{
    ColorDef{"Blue", QT_TR_NOOP("Blue"), 0x4E7CF6, 0, kFree, 0},
    ColorDef{"LightBlue", QT_TR_NOOP("Light blue"), 0x5BC8F5, 0, kAchievement, 0},
    ColorDef{"Purple", QT_TR_NOOP("Purple"), 0x7B4FC9, 0, kShop, 50},
    ColorDef{"Pink", QT_TR_NOOP("Pink"), 0xE86AA8, 0, kShop, 60},
    ColorDef{"Red", QT_TR_NOOP("Red"), 0xE7471D, 0, kAchievement, 0},
    ColorDef{"Orange", QT_TR_NOOP("Orange"), 0xF5A623, 0, kAchievement, 0},
    ColorDef{"Yellow", QT_TR_NOOP("Yellow"), 0xF7D23E, 0, kAchievement, 0},
    ColorDef{"Green", QT_TR_NOOP("Green"), 0x58A828, 0, kShop, 80},
    ColorDef{"Black", QT_TR_NOOP("Black"), 0x3A3A44, 0, kAchievement, 0},
    ColorDef{"White", QT_TR_NOOP("White"), 0xF2F2F2, 0, kShop, 150},
};

constexpr std::array kMapColors{
    ColorDef{"Sand", QT_TR_NOOP("Sand"), 0xC9A25F, 0xB98F4C, kFree, 0},
    ColorDef{"Clay", QT_TR_NOOP("Clay"), 0xB5654A, 0xA5553B, kAchievement, 0},
    ColorDef{"Copper", QT_TR_NOOP("Copper"), 0xC27C45, 0xB06E3B, kShop, 40},
    ColorDef{"Burgundy", QT_TR_NOOP("Burgundy"), 0x8E3A4A, 0x7A2E3C, kAchievement, 0},
    ColorDef{"Mauve", QT_TR_NOOP("Mauve"), 0x8F7390, 0x7E6380, kShop, 60},
    ColorDef{"Night", QT_TR_NOOP("Night"), 0x4B5573, 0x3D4663, kAchievement, 0},
    ColorDef{"Slate", QT_TR_NOOP("Slate"), 0x6F8190, 0x5F717F, kShop, 70},
    ColorDef{"Ash", QT_TR_NOOP("Ash"), 0xA8A8A8, 0x969696, kAchievement, 0},
    ColorDef{"Charcoal", QT_TR_NOOP("Charcoal"), 0x4A4540, 0x3E3A36, kAchievement, 0},
    ColorDef{"Bone", QT_TR_NOOP("Bone"), 0xD9CBA8, 0xC9BB98, kShop, 100},
};

constexpr const char* kMedal = "achievements";
constexpr const char* kTrophy = "trophy";

constexpr AchievementDef colorAchievement(const char* id, const char* title, const char* description,
                                          ColorKind kind, const char* colorId)
{
    return {id, title, description, kMedal, kind, colorId, 0};
}

constexpr AchievementDef winAchievement(const char* id, const char* title, const char* description, int drumsticks)
{
    return {id, title, description, kTrophy, ColorKind::Snake, nullptr, drumsticks};
}

constexpr std::array kAchievements{
    colorAchievement("Eat1", QT_TR_NOOP("First Bite"), QT_TR_NOOP("Eat 1 drumstick"), ColorKind::Snake, "Red"),
    colorAchievement("Eat3", QT_TR_NOOP("Appetizer"), QT_TR_NOOP("Eat 3 drumsticks in total"), ColorKind::Map, "Clay"),
    colorAchievement("Eat5", QT_TR_NOOP("Hungry"), QT_TR_NOOP("Eat 5 drumsticks in total"), ColorKind::Snake, "LightBlue"),
    colorAchievement("Eat10", QT_TR_NOOP("Full Menu"), QT_TR_NOOP("Eat 10 drumsticks in total"), ColorKind::Map, "Night"),
    colorAchievement("Eat25", QT_TR_NOOP("Pollos Family"), QT_TR_NOOP("Eat 25 drumsticks in total"), ColorKind::Snake, "Yellow"),
    colorAchievement("Eat50", QT_TR_NOOP("Head Chef"), QT_TR_NOOP("Eat 50 drumsticks in total"), ColorKind::Map, "Burgundy"),
    colorAchievement("Eat100", QT_TR_NOOP("Pollos Empire"), QT_TR_NOOP("Eat 100 drumsticks in total"), ColorKind::Snake, "Black"),
    colorAchievement("Die1", QT_TR_NOOP("First Lesson"), QT_TR_NOOP("Lose a round"), ColorKind::Map, "Ash"),
    colorAchievement("Die5", QT_TR_NOOP("Stubborn"), QT_TR_NOOP("Die 5 times"), ColorKind::Snake, "Orange"),
    colorAchievement("Die25", QT_TR_NOOP("Never Give Up"), QT_TR_NOOP("Die 25 times"), ColorKind::Map, "Charcoal"),
    winAchievement("Win1", QT_TR_NOOP("Victory!"), QT_TR_NOOP("Win a round"), 30),
    winAchievement("Win5", QT_TR_NOOP("Champion"), QT_TR_NOOP("Win 5 rounds"), 60),
    winAchievement("Win10", QT_TR_NOOP("Legend"), QT_TR_NOOP("Win 10 rounds"), 100),
};

template <typename Range>
int indexOfId(const Range& range, const QString& id)
{
    const auto it = std::ranges::find_if(range, [&](const auto& item) { return id == QLatin1StringView(item.id); });
    return it == range.end() ? -1 : static_cast<int>(it - range.begin());
}

int indexOfName(const auto& names, const QString& name)
{
    const auto it = std::ranges::find_if(names, [&](const char* item) { return name == QLatin1StringView(item); });
    return it == names.end() ? -1 : static_cast<int>(it - names.begin());
}

ArtSpec colorArt(ColorKind kind, const ColorDef& color)
{
    if (kind == ColorKind::Map)
    {
        return {QStringLiteral("size_3x3"),
                {{QStringLiteral("#C9A25F"), QColor(color.base).name()},
                 {QStringLiteral("#B98F4C"), QColor(color.shade).name()}}};
    }

    const QColor base(color.base);
    return {QStringLiteral("sphere"),
            {{QStringLiteral("#4E7CF6"), base.name()},
             {QStringLiteral("#2B3FA8"), base.darker(160).name()},
             {QStringLiteral("#8FB0FF"), base.lighter(150).name()}}};
}
} // namespace

namespace Catalog
{
std::span<const ColorDef> colors(ColorKind kind)
{
    return kind == ColorKind::Snake ? std::span<const ColorDef>(kSnakeColors) : std::span<const ColorDef>(kMapColors);
}

std::span<const AchievementDef> achievements()
{
    return kAchievements;
}

int colorIndex(ColorKind kind, const QString& id)
{
    return indexOfId(colors(kind), id);
}

int achievementIndex(const QString& id)
{
    return indexOfId(kAchievements, id);
}

int boardSizeIndex(const QString& id)
{
    return indexOfName(kBoardSizes, id);
}

int speedIndex(const QString& id)
{
    return indexOfName(kSpeeds, id);
}

int fruitCountIndex(int count)
{
    const auto it = std::ranges::find(kFruitCounts, count);
    return it == kFruitCounts.end() ? -1 : static_cast<int>(it - kFruitCounts.begin());
}

QString defaultColorId(ColorKind kind)
{
    const auto list = colors(kind);
    const auto it = std::ranges::find(list, kFree, &ColorDef::source);
    return QLatin1StringView(it->id);
}

QString text(const char* source)
{
    return QCoreApplication::translate("Catalog", source);
}

ItemInfo colorInfo(ColorKind kind, const QString& id)
{
    const ColorDef& color = colors(kind)[colorIndex(kind, id)];
    const char* pattern = kind == ColorKind::Snake ? "%1 snake" : "%1 map";
    return {QCoreApplication::translate("Catalog", pattern).arg(text(color.name)), colorArt(kind, color), color.source,
            color.price};
}

ItemInfo rewardInfo(const AchievementDef& achievement)
{
    if (achievement.rewardColorId == nullptr)
    {
        return {QCoreApplication::translate("Catalog", "+%1 drumsticks").arg(achievement.rewardDrumsticks),
                QStringLiteral("drumstick")};
    }
    return colorInfo(achievement.rewardKind, QLatin1StringView(achievement.rewardColorId));
}

bool isConsistent()
{
    const auto grantedBy = [](ColorKind kind, const char* id) {
        return std::ranges::count_if(kAchievements, [&](const AchievementDef& def) {
            return def.rewardColorId != nullptr && def.rewardKind == kind
                   && QLatin1StringView(def.rewardColorId) == QLatin1StringView(id);
        });
    };

    for (const ColorKind kind : kColorKinds)
    {
        for (const ColorDef& color : colors(kind))
        {
            if ((grantedBy(kind, color.id) == 1) != (color.source == ColorSource::Achievement))
                return false;
        }
    }

    return std::ranges::all_of(kAchievements, [](const AchievementDef& def) {
        return def.rewardColorId != nullptr ? colorIndex(def.rewardKind, QLatin1StringView(def.rewardColorId)) >= 0
                                            : def.rewardDrumsticks > 0;
    });
}
} // namespace Catalog
