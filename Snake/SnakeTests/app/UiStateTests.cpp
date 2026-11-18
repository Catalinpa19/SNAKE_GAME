#include "app/UiState.h"

#include <gtest/gtest.h>

TEST(UiState, StartsWithTheCatalogDefaults)
{
    const UiState state;

    EXPECT_EQ(state.settings.mode, Catalog::kDefaultMode);
    EXPECT_EQ(state.settings.board, Catalog::kDefaultBoardSize);
    EXPECT_EQ(state.settings.speed, Catalog::kDefaultSpeed);
    EXPECT_EQ(state.settings.appleCount, 1);
    EXPECT_EQ(state.applesBalance, 0);
    EXPECT_EQ(state.bestScore, 0);
    EXPECT_TRUE(state.unlockedAchievements.isEmpty());
}

TEST(UiState, OnlyFreeColorsAreUnlockedAndOneIsEquipped)
{
    const UiState state;

    for (const ColorKind kind : kColorKinds)
    {
        const auto colors = Catalog::colors(kind);
        for (int i = 0; i < static_cast<int>(colors.size()); ++i)
            EXPECT_EQ(state.unlocked(kind).contains(i), colors[i].source == ColorSource::Default);

        EXPECT_TRUE(state.unlocked(kind).contains(state.equipped(kind)));
    }
}
