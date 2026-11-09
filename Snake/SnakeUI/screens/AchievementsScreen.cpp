#include "screens/AchievementsScreen.h"

#include "app/Catalog.h"
#include "app/ProfileStore.h"
#include "widgets/ListRow.h"

namespace
{
constexpr QSize kHeaderArtSize(32, 42);
constexpr QSize kIconSize(48, 60);
constexpr QSize kRewardArtSize(24, 24);
} // namespace

AchievementsScreen::AchievementsScreen(const ProfileData& profile, QWidget* parent)
    : ListScreen(QStringLiteral("trophy"), kHeaderArtSize, parent)
{
    refresh(profile);
}

void AchievementsScreen::refresh(const ProfileData& profile)
{
    clearRows();

    int unlockedCount = 0;
    for (const AchievementDef& def : Catalog::achievements())
    {
        const bool unlocked = profile.unlockedAchievements.contains(QLatin1StringView(def.id));
        unlockedCount += unlocked ? 1 : 0;

        const ItemInfo reward = Catalog::rewardInfo(def);
        auto* row = new ListRow(ArtSpec(QLatin1StringView(def.icon)), kIconSize, !unlocked);
        row->addTitle(Catalog::text(def.title));
        row->addText(Catalog::text(def.description));
        row->addArtLine(tr("Unlocks:"), reward.art, kRewardArtSize, reward.label);
        addRow(row);
    }
    endRows();

    setHeaderText(QStringLiteral("%1 / %2").arg(unlockedCount).arg(Catalog::achievements().size()));
}
