#include "screens/ShopScreen.h"

#include "app/UiState.h"

#include <QLabel>
#include <QPushButton>

#include <gmock/gmock.h>

#include <algorithm>
#include <utility>
#include <vector>

namespace
{
struct ShopEntry
{
    ColorKind kind;
    int color;
    int price;
};

// The colors the catalog sells, cheapest first.
std::vector<ShopEntry> entriesForSale()
{
    std::vector<ShopEntry> entries;
    for (const ColorKind kind : kColorKinds)
    {
        const auto colors = Catalog::colors(kind);
        for (int i = 0; i < static_cast<int>(colors.size()); ++i)
        {
            if (colors[i].source == ColorSource::Shop)
                entries.push_back({kind, i, colors[i].price});
        }
    }
    std::stable_sort(entries.begin(), entries.end(), [](const ShopEntry& a, const ShopEntry& b) { return a.price < b.price; });
    return entries;
}
} // namespace

TEST(ShopScreen, UnaffordableButtonsAreDisabled)
{
    UiState state;
    ShopScreen screen(state);

    const auto buttons = screen.findChildren<QPushButton*>("buyButton");
    ASSERT_FALSE(buttons.isEmpty());
    for (auto* button : buttons)
        EXPECT_FALSE(button->isEnabled());
}

TEST(ShopScreen, SellsOnlyShopColorsInAscendingPriceOrder)
{
    UiState state;
    state.applesBalance = 1000000;
    ShopScreen screen(state);

    const auto entries = entriesForSale();
    const auto buttons = screen.findChildren<QPushButton*>("buyButton");
    ASSERT_EQ(static_cast<size_t>(buttons.size()), entries.size());

    std::vector<std::pair<ColorKind, int>> requested;
    QObject::connect(&screen, &ShopScreen::purchaseRequested, &screen,
                     [&](ColorKind kind, int color) { requested.emplace_back(kind, color); });

    for (auto* button : buttons)
        button->click();

    std::vector<std::pair<ColorKind, int>> expected;
    for (const ShopEntry& entry : entries)
        expected.emplace_back(entry.kind, entry.color);
    EXPECT_THAT(requested, ::testing::ElementsAreArray(expected));
}

TEST(ShopScreen, RefreshRemovesOwnedItemsWithoutDuplicates)
{
    UiState state;
    ShopScreen screen(state);

    for (const ColorKind kind : kColorKinds)
    {
        for (int i = 0; i < static_cast<int>(Catalog::colors(kind).size()); ++i)
            state.unlocked(kind).append(i);
    }
    screen.refresh(state);
    screen.refresh(state);

    EXPECT_TRUE(screen.findChildren<QPushButton*>("buyButton").isEmpty());

    int messages = 0;
    for (auto* label : screen.findChildren<QLabel*>())
        messages += label->text() == "Everything is unlocked!" ? 1 : 0;
    EXPECT_EQ(messages, 1);
}