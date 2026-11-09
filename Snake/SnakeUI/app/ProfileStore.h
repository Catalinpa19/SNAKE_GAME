#pragma once

#include "app/Catalog.h"

#include <QMap>
#include <QString>
#include <QStringList>

#include <array>

struct ProfileSettings
{
    QString mode = QLatin1StringView(Catalog::kDefaultMode);
    QString board = QLatin1StringView(Catalog::kDefaultBoardSize);
    QString speed = QLatin1StringView(Catalog::kDefaultSpeed);
    int appleCount = 1;
    bool audioEnabled = true;
};

struct ProfileData
{
    int applesBalance = 0;
    int applesLifetime = 0;
    QMap<QString, int> highScores; // key: "<mode>/<board>"
    QStringList unlockedAchievements;
    ProfileSettings settings;

    std::array<QString, kColorKinds.size()> equippedColors;
    std::array<QStringList, kColorKinds.size()> unlockedColors;

    QString& equipped(ColorKind kind) { return equippedColors[static_cast<size_t>(kind)]; }
    const QString& equipped(ColorKind kind) const { return equippedColors[static_cast<size_t>(kind)]; }
    QStringList& unlocked(ColorKind kind) { return unlockedColors[static_cast<size_t>(kind)]; }
    const QStringList& unlocked(ColorKind kind) const { return unlockedColors[static_cast<size_t>(kind)]; }

    // LOGIC: IProgressService::highScore(mode, board).
    int bestScore() const;
};

// LOGIC: this whole class is an interim replacement for SnakeLogic. The file format follows the profile
// from the project structure, so IProfileRepository / JsonProfileRepository can take over the same file.
// Mapping: unlockAchievement -> IAchievementService, purchaseColor -> ISkinService, the balance and the
// high scores -> IProgressService. Only the "cosmetics" block and the settings stay UI related.
class ProfileStore
{
public:
    explicit ProfileStore(QString path);

    static QString defaultPath();

    ProfileData& data() { return m_data; }
    const ProfileData& data() const { return m_data; }

    bool save() const;

    // Returns false if the achievement is unknown or already unlocked.
    bool unlockAchievement(const QString& id);

    // Returns false if the color is already owned, not sold in the shop, or too expensive.
    bool purchaseColor(ColorKind kind, const QString& id);

private:
    void load();
    void sanitize();
    void unlockColor(ColorKind kind, const QString& id);
    void applyReward(const AchievementDef& achievement, bool payDrumsticks);

    QString m_path;
    ProfileData m_data;
};
