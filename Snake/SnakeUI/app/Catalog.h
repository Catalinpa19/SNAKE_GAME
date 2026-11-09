#pragma once

#include "widgets/ArtImage.h"

#include <QColor>
#include <QString>

#include <array>
#include <span>

// LOGIC: this catalog is UI-side only for now. The achievement conditions and rewards, the unlockable skins
// and their prices belong to SnakeLogic (IAchievementService, ISkinService). Once they exist, the UI should
// keep only the presentation data here (names, art) and read the rest through the api interfaces.

enum class ColorKind
{
    Snake,
    Map
};

inline constexpr std::array kColorKinds{ColorKind::Snake, ColorKind::Map};

enum class ColorSource
{
    Default,
    Achievement,
    Shop
};

struct ColorDef
{
    const char* id;
    const char* name;
    QRgb base;  // the snake color, or the light tile of the map
    QRgb shade; // the dark tile of the map, unused for the snake
    ColorSource source;
    int price; // only used when the source is Shop
};

struct AchievementDef
{
    const char* id;
    const char* title;
    const char* description;
    const char* icon;
    ColorKind rewardKind; // unused when the reward is drumsticks
    const char* rewardColorId; // nullptr when the reward is drumsticks
    int rewardDrumsticks;
};

struct ItemInfo
{
    QString label;
    ArtSpec art;
    ColorSource source = ColorSource::Default;
    int price = 0;
};

namespace Catalog
{
inline constexpr std::array kFruitCounts{1, 3, 5, 10};
inline constexpr std::array kSpeeds{"Slow", "Normal", "Fast"};
inline constexpr std::array kBoardSizes{"Small", "Medium", "Large"};

inline constexpr const char* kDefaultMode = "Classic";
inline constexpr const char* kDefaultSpeed = "Normal";
inline constexpr const char* kDefaultBoardSize = "Medium";

std::span<const ColorDef> colors(ColorKind kind);
std::span<const AchievementDef> achievements();

// Index in the list, or -1 when the id is unknown.
int colorIndex(ColorKind kind, const QString& id);
int achievementIndex(const QString& id);
int boardSizeIndex(const QString& id);
int speedIndex(const QString& id);
int fruitCountIndex(int count);

QString defaultColorId(ColorKind kind);
QString text(const char* source);

// The id must exist in the color list of that kind.
ItemInfo colorInfo(ColorKind kind, const QString& id);
ItemInfo rewardInfo(const AchievementDef& achievement);

// Every color has one source, and every achievement color is granted by exactly one achievement.
bool isConsistent();
} // namespace Catalog
