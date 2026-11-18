#include "widgets/OptionButton.h"
#include "widgets/OptionRow.h"

#include "app/Catalog.h"

#include <gmock/gmock.h>

#include <vector>

using ::testing::ElementsAre;
using ::testing::Each;
using ::testing::IsEmpty;
using ::testing::SizeIs;

namespace
{
const QSize kCell(52, 52);
constexpr int kArtBox = 50;
const QString kHint = "unlock it";

// The options of the snake color row, built from the catalog like the settings screen does:
// only the free colors are unlocked.
QList<OptionSpec> snakeColorOptions()
{
    QList<OptionSpec> options;
    const auto colors = Catalog::colors(ColorKind::Snake);
    for (int i = 0; i < static_cast<int>(colors.size()); ++i)
    {
        const ItemInfo info = Catalog::colorInfo(ColorKind::Snake, i);
        options.append({info.art, info.label, colors[i].source != ColorSource::Default, kHint});
    }
    return options;
}

QList<OptionButton*> cells(OptionRow& row)
{
    QList<OptionButton*> result;
    for (auto* button : row.findChildren<QAbstractButton*>())
    {
        if (auto* cell = dynamic_cast<OptionButton*>(button))
            result.append(cell);
    }
    return result;
}

void recordSelections(OptionRow& row, std::vector<int>& selections)
{
    QObject::connect(&row, &OptionRow::selected, &row, [&selections](int index) { selections.push_back(index); });
}
} // namespace

TEST(OptionRow, OnlyFreeColorsAreEnabledAndLockedOnesExplainHowToUnlockThem)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    const auto list = cells(row);
    const ItemInfo second = Catalog::colorInfo(ColorKind::Snake, 1);

    ASSERT_EQ(list.size(), static_cast<int>(Catalog::colors(ColorKind::Snake).size()));
    EXPECT_TRUE(list[0]->isChecked());
    EXPECT_TRUE(list[0]->isEnabled());
    EXPECT_FALSE(list[1]->isEnabled());
    EXPECT_EQ(list[1]->toolTip(), second.label + " (" + kHint + ")");
}

TEST(OptionRow, SelectUpdatesExclusiveSelectionAndEmits)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    row.setLocked(1, false);
    std::vector<int> selections;
    recordSelections(row, selections);

    row.select(1);

    EXPECT_THAT(selections, ElementsAre(1));
    EXPECT_TRUE(cells(row)[1]->isChecked());
    EXPECT_FALSE(cells(row)[0]->isChecked());
}

TEST(OptionRow, ClickEmitsMatchingIndex)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    row.setLocked(1, false);
    std::vector<int> selections;
    recordSelections(row, selections);

    cells(row)[1]->click();

    EXPECT_THAT(selections, ElementsAre(1));
}

TEST(OptionRow, LockedCellDoesNotEmitOnClick)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    std::vector<int> selections;
    recordSelections(row, selections);

    cells(row)[1]->click();

    EXPECT_THAT(selections, IsEmpty());
}

class RejectedIndexTest : public ::testing::TestWithParam<int>
{
};

TEST_P(RejectedIndexTest, InvalidOrLockedIndexDoesNotEmit)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    std::vector<int> selections;
    recordSelections(row, selections);

    row.select(GetParam());

    EXPECT_THAT(selections, IsEmpty());
    EXPECT_TRUE(cells(row)[0]->isChecked());
}

INSTANTIATE_TEST_SUITE_P(Indices, RejectedIndexTest, ::testing::Values(-1, 1, 5, 100));

TEST(OptionRow, UnlockingRestoresTooltipAndCursorAndLockingUndoesIt)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    auto* cell = cells(row)[1];

    row.setLocked(1, false);
    EXPECT_TRUE(cell->isEnabled());
    EXPECT_EQ(cell->toolTip(), Catalog::colorInfo(ColorKind::Snake, 1).label);
    EXPECT_EQ(cell->cursor().shape(), Qt::PointingHandCursor);

    row.setLocked(1, true);
    EXPECT_FALSE(cell->isEnabled());
    EXPECT_EQ(cell->cursor().shape(), Qt::ArrowCursor);

    row.setLocked(-1, false);
    row.setLocked(100, false);
}

TEST(OptionRow, RandomSelectPicksOnlyUnlockedCells)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    std::vector<int> selections;
    recordSelections(row, selections);

    for (int i = 0; i < 20; ++i)
        row.selectRandomUnlocked();

    EXPECT_THAT(selections, SizeIs(20));
    EXPECT_THAT(selections, Each(0));
}

TEST(OptionRow, RandomSelectWithEverythingLockedDoesNothing)
{
    OptionRow row(snakeColorOptions(), kCell, kArtBox);
    row.setLocked(0, true);
    std::vector<int> selections;
    recordSelections(row, selections);

    row.selectRandomUnlocked();

    EXPECT_THAT(selections, IsEmpty());
}

TEST(OptionRow, SelectingAnOffscreenCellRotatesItIntoView)
{
    constexpr int kVisible = 5;
    OptionRow row(snakeColorOptions(), kCell, kArtBox, 0, kVisible);
    row.setLocked(7, false);

    EXPECT_FALSE(cells(row)[0]->isHidden());
    EXPECT_TRUE(cells(row)[7]->isHidden());

    row.select(7);

    EXPECT_FALSE(cells(row)[7]->isHidden());
}