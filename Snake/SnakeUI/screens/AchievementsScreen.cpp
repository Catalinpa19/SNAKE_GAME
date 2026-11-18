#include "screens/AchievementsScreen.h"

#include "app/Catalog.h"
#include "app/NumberFormat.h"
#include "app/UiState.h"
#include "widgets/ListRow.h"

namespace
{
constexpr QSize kHeaderArtSize(32, 42);
constexpr QSize kIconSize(48, 60);
constexpr QSize kRewardArtSize(24, 24);
} // namespace

AchievementsScreen::AchievementsScreen(const UiState& state, QWidget* parent)
    : ListScreen(QStringLiteral("trophy"), kHeaderArtSize, parent)
{
    refresh(state);
}

void AchievementsScreen::refresh(const UiState& state)
{
    clearRows();

    int unlockedCount = 0;
    const auto achievements = Catalog::achievements();
    for (int i = 0; i < static_cast<int>(achievements.size()); ++i)
    {
        const AchievementDef& def = achievements[i];
        const bool unlocked = state.unlockedAchievements.contains(i);
        unlockedCount += unlocked ? 1 : 0;

        const ItemInfo reward = Catalog::rewardInfo(def);
        auto* row = new ListRow(ArtSpec(QLatin1StringView(def.icon)), kIconSize, !unlocked);
        row->addTitle(Catalog::text(def.title));
        row->addText(Catalog::text(def.description));
        row->addArtLine(tr("Unlocks:"), reward.art, kRewardArtSize, reward.label);
        addRow(row);
    }
    endRows();

    setHeaderText(QStringLiteral("%1 / %2").arg(formatCount(unlockedCount), formatCount(Catalog::achievements().size())));
}
