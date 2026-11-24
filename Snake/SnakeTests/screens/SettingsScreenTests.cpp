#include "screens/SettingsScreen.h"

#include "app/UiState.h"
#include "support/TestHelpers.h"
#include "widgets/OptionButton.h"
#include "widgets/OptionRow.h"

#include <gmock/gmock.h>

#include <algorithm>
#include <utility>
#include <vector>

namespace
{
// Rows in creation order: fruit, mode, fruit count, speed, board size, snake colors, map colors.
constexpr int kRowCount = 7;
constexpr int kFruitCountRow = 2;
constexpr int kSpeedRow = 3;
constexpr int kBoardRow = 4;
constexpr int kFirstColorRow = 5;

int enabledCells(OptionRow* row)
{
    int count = 0;
    for (auto* button : row->findChildren<QAbstractButton*>())
        count += dynamic_cast<OptionButton*>(button) && button->isEnabled() ? 1 : 0;
    return count;
}

class FruitCountTest : public ::testing::TestWithParam<int>
{
};

class SpeedAndBoardTest : public ::testing::TestWithParam<int>
{
};
} // namespace

TEST_P(FruitCountTest, SignalUsesTheCatalogValue)
{
    UiState state;
    SettingsScreen screen(state);
    const auto rows = screen.findChildren<OptionRow*>();
    ASSERT_EQ(rows.size(), kRowCount);

    std::vector<int> counts;
    QObject::connect(&screen, &SettingsScreen::fruitCountSelected, &screen, [&](int count) { counts.push_back(count); });

    rows[kFruitCountRow]->select(GetParam());

    EXPECT_THAT(counts, ::testing::ElementsAre(Catalog::kFruitCounts[GetParam()]));
}

INSTANTIATE_TEST_SUITE_P(Counts, FruitCountTest, ::testing::Values(0, 1, 2, 3));

TEST_P(SpeedAndBoardTest, SignalsUseTheCatalogValues)
{
    UiState state;
    SettingsScreen screen(state);
    const auto rows = screen.findChildren<OptionRow*>();

    QStringList speeds, boards;
    QObject::connect(&screen, &SettingsScreen::speedSelected, &screen, [&](const QString& value) { speeds << value; });
    QObject::connect(&screen, &SettingsScreen::boardSizeSelected, &screen, [&](const QString& value) { boards << value; });

    rows[kSpeedRow]->select(GetParam());
    rows[kBoardRow]->select(GetParam());

    EXPECT_THAT(speeds, ::testing::ElementsAre(QString(Catalog::kSpeeds[GetParam()])));
    EXPECT_THAT(boards, ::testing::ElementsAre(QString(Catalog::kBoardSizes[GetParam()])));
}

INSTANTIATE_TEST_SUITE_P(Choices, SpeedAndBoardTest, ::testing::Values(0, 1, 2));

TEST(SettingsScreen, BackEmitsTheRequest)
{
    UiState state;
    SettingsScreen screen(state);
    int requests = 0;
    QObject::connect(&screen, &PanelScreen::backRequested, &screen, [&] { ++requests; });

    auto* back = findButton(screen, "Back");
    ASSERT_NE(back, nullptr);
    back->click();

    EXPECT_EQ(requests, 1);
}

TEST(SettingsScreen, OnlyFreeColorsAreSelectableAtFirst)
{
    UiState state;
    SettingsScreen screen(state);
    const auto rows = screen.findChildren<OptionRow*>();

    for (const ColorKind kind : kColorKinds)
    {
        const auto colors = Catalog::colors(kind);
        const int free = static_cast<int>(std::count_if(colors.begin(), colors.end(), [](const ColorDef& color) {
            return color.source == ColorSource::Default;
        }));
        EXPECT_EQ(enabledCells(rows[kFirstColorRow + static_cast<int>(kind)]), free);
    }
}

TEST(SettingsScreen, RefreshUnlocksColorsAndSelectionEmitsTheEnumValue)
{
    UiState state;
    SettingsScreen screen(state);
    const auto rows = screen.findChildren<OptionRow*>();

    std::vector<std::pair<ColorKind, int>> selected;
    QObject::connect(&screen, &SettingsScreen::colorSelected, &screen,
                     [&](ColorKind kind, int color) { selected.emplace_back(kind, color); });

    for (const ColorKind kind : kColorKinds)
    {
        const int count = static_cast<int>(Catalog::colors(kind).size());
        for (int color = 0; color < count; ++color)
        {
            if (!state.unlocked(kind).contains(color))
                state.unlocked(kind).append(color);
            screen.refresh(state);

            selected.clear();
            rows[kFirstColorRow + static_cast<int>(kind)]->select(color);

            EXPECT_THAT(selected, ::testing::ElementsAre(std::make_pair(kind, color)));
        }
    }
}