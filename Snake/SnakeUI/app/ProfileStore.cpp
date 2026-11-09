#include "app/ProfileStore.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>

#include <algorithm>
#include <utility>

using namespace Qt::StringLiterals;

namespace
{
constexpr int kVersion = 1;

struct ColorKeys
{
    QLatin1StringView equipped;
    QLatin1StringView unlocked;
};

ColorKeys colorKeys(ColorKind kind)
{
    return kind == ColorKind::Snake ? ColorKeys{"snakeColor"_L1, "unlockedSnakeColors"_L1}
                                    : ColorKeys{"mapColor"_L1, "unlockedMapColors"_L1};
}

QString scoreKey(const QString& mode, const QString& board)
{
    return mode + u'/' + board;
}

QStringList toStringList(const QJsonValue& value)
{
    QStringList list;
    for (const QJsonValue& item : value.toArray())
        list.append(item.toString());
    return list;
}

QStringList freeColorIds(ColorKind kind)
{
    QStringList ids;
    for (const ColorDef& color : Catalog::colors(kind))
    {
        if (color.source == ColorSource::Default)
            ids.append(QLatin1StringView(color.id));
    }
    return ids;
}

bool isAchievementColor(ColorKind kind, const QString& id)
{
    const int index = Catalog::colorIndex(kind, id);
    return index >= 0 && Catalog::colors(kind)[index].source == ColorSource::Achievement;
}
} // namespace

int ProfileData::bestScore() const
{
    return highScores.value(scoreKey(settings.mode, settings.board), 0);
}

ProfileStore::ProfileStore(QString path)
    : m_path(std::move(path))
{
    load();
    sanitize();
}

QString ProfileStore::defaultPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + u"/profile.json"_s;
}

void ProfileStore::load()
{
    QFile file(m_path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject())
        return;

    const QJsonObject root = document.object();
    m_data.applesBalance = std::max(0, root.value("applesBalance"_L1).toInt());
    m_data.applesLifetime = std::max(0, root.value("applesLifetime"_L1).toInt());
    m_data.unlockedAchievements = toStringList(root.value("unlockedAchievements"_L1));

    for (const QJsonValue& item : root.value("highScores"_L1).toArray())
    {
        const QJsonObject entry = item.toObject();
        const QString key = scoreKey(entry.value("mode"_L1).toString(), entry.value("board"_L1).toString());
        m_data.highScores[key] = std::max(0, entry.value("best"_L1).toInt());
    }

    const QJsonObject settings = root.value("settings"_L1).toObject();
    m_data.settings.mode = settings.value("mode"_L1).toString(m_data.settings.mode);
    m_data.settings.board = settings.value("board"_L1).toString(m_data.settings.board);
    m_data.settings.speed = settings.value("speed"_L1).toString(m_data.settings.speed);
    m_data.settings.appleCount = settings.value("appleCount"_L1).toInt(m_data.settings.appleCount);
    m_data.settings.audioEnabled = settings.value("audioEnabled"_L1).toBool(m_data.settings.audioEnabled);

    const QJsonObject cosmetics = root.value("cosmetics"_L1).toObject();
    for (const ColorKind kind : kColorKinds)
    {
        const ColorKeys keys = colorKeys(kind);
        m_data.equipped(kind) = cosmetics.value(keys.equipped).toString();
        m_data.unlocked(kind) = toStringList(cosmetics.value(keys.unlocked));
    }
}

bool ProfileStore::save() const
{
    QDir().mkpath(QFileInfo(m_path).absolutePath());

    QJsonArray scores;
    for (auto it = m_data.highScores.cbegin(); it != m_data.highScores.cend(); ++it)
    {
        const QStringList parts = it.key().split(u'/');
        if (parts.size() == 2)
            scores.append(QJsonObject{{"mode"_L1, parts[0]}, {"board"_L1, parts[1]}, {"best"_L1, it.value()}});
    }

    QJsonObject cosmetics;
    for (const ColorKind kind : kColorKinds)
    {
        const ColorKeys keys = colorKeys(kind);
        cosmetics.insert(keys.equipped, m_data.equipped(kind));
        cosmetics.insert(keys.unlocked, QJsonArray::fromStringList(m_data.unlocked(kind)));
    }

    const QJsonObject root{
        {"version"_L1, kVersion},
        {"applesBalance"_L1, m_data.applesBalance},
        {"applesLifetime"_L1, m_data.applesLifetime},
        {"unlockedAchievements"_L1, QJsonArray::fromStringList(m_data.unlockedAchievements)},
        {"highScores"_L1, scores},
        {"settings"_L1,
         QJsonObject{{"mode"_L1, m_data.settings.mode},
                     {"board"_L1, m_data.settings.board},
                     {"speed"_L1, m_data.settings.speed},
                     {"appleCount"_L1, m_data.settings.appleCount},
                     {"audioEnabled"_L1, m_data.settings.audioEnabled}}},
        {"cosmetics"_L1, cosmetics},
    };

    QSaveFile file(m_path);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return file.commit();
}

bool ProfileStore::unlockAchievement(const QString& id)
{
    const int index = Catalog::achievementIndex(id);
    if (index < 0 || m_data.unlockedAchievements.contains(id))
        return false;

    m_data.unlockedAchievements.append(id);
    applyReward(Catalog::achievements()[index], true);
    return true;
}

bool ProfileStore::purchaseColor(ColorKind kind, const QString& id)
{
    if (Catalog::colorIndex(kind, id) < 0 || m_data.unlocked(kind).contains(id))
        return false;

    const ItemInfo info = Catalog::colorInfo(kind, id);
    if (info.source != ColorSource::Shop || m_data.applesBalance < info.price)
        return false;

    m_data.applesBalance -= info.price;
    unlockColor(kind, id);
    return true;
}

void ProfileStore::unlockColor(ColorKind kind, const QString& id)
{
    QStringList& unlocked = m_data.unlocked(kind);
    if (!unlocked.contains(id))
        unlocked.append(id);
}

void ProfileStore::applyReward(const AchievementDef& achievement, bool payDrumsticks)
{
    if (achievement.rewardColorId != nullptr)
        unlockColor(achievement.rewardKind, QLatin1StringView(achievement.rewardColorId));
    else if (payDrumsticks)
        m_data.applesBalance += achievement.rewardDrumsticks;
}

void ProfileStore::sanitize()
{
    // Colors earned by achievements are re-derived from the achievements, never trusted from the file.
    for (const ColorKind kind : kColorKinds)
        m_data.unlocked(kind).removeIf([kind](const QString& id) { return isAchievementColor(kind, id); });

    const QStringList stored = std::exchange(m_data.unlockedAchievements, {});
    for (const QString& id : stored)
    {
        const int index = Catalog::achievementIndex(id);
        if (index < 0 || m_data.unlockedAchievements.contains(id))
            continue;
        m_data.unlockedAchievements.append(id);
        applyReward(Catalog::achievements()[index], false);
    }

    for (const ColorKind kind : kColorKinds)
    {
        QStringList unlocked = freeColorIds(kind);
        for (const QString& id : std::exchange(m_data.unlocked(kind), {}))
        {
            if (Catalog::colorIndex(kind, id) >= 0 && !unlocked.contains(id))
                unlocked.append(id);
        }
        m_data.unlocked(kind) = unlocked;

        if (!unlocked.contains(m_data.equipped(kind)))
            m_data.equipped(kind) = Catalog::defaultColorId(kind);
    }

    ProfileSettings& settings = m_data.settings;
    const ProfileSettings defaults;
    if (Catalog::boardSizeIndex(settings.board) < 0)
        settings.board = defaults.board;
    if (Catalog::speedIndex(settings.speed) < 0)
        settings.speed = defaults.speed;
    if (Catalog::fruitCountIndex(settings.appleCount) < 0)
        settings.appleCount = defaults.appleCount;
    settings.mode = defaults.mode;
}
