#include "screens/ShopScreen.h"

#include "app/NumberFormat.h"
#include "app/UiState.h"
#include "widgets/ListRow.h"

#include <QLabel>
#include <QPushButton>

#include <algorithm>

namespace
{
constexpr QSize kHeaderArtSize(32, 32);
constexpr QSize kSwatchSize(48, 48);
constexpr QSize kPriceArtSize(16, 16);
constexpr QSize kBuyButtonSize(76, 36);

struct ShopItem
{
    ColorKind kind;
    int color;
    ItemInfo info;
};

QList<ShopItem> itemsForSale(const UiState& state)
{
    QList<ShopItem> items;
    for (const ColorKind kind : kColorKinds)
    {
        const auto colors = Catalog::colors(kind);
        for (int i = 0; i < static_cast<int>(colors.size()); ++i)
        {
            if (colors[i].source == ColorSource::Shop && !state.unlocked(kind).contains(i))
                items.append({kind, i, Catalog::colorInfo(kind, i)});
        }
    }
    std::ranges::stable_sort(items, {}, [](const ShopItem& item) { return item.info.price; });
    return items;
}
} // namespace

ShopScreen::ShopScreen(const UiState& state, QWidget* parent)
    : ListScreen(QStringLiteral("drumstick"), kHeaderArtSize, parent)
{
    refresh(state);
}

void ShopScreen::refresh(const UiState& state)
{
    clearRows();
    setHeaderText(formatCount(state.applesBalance));

    const QList<ShopItem> items = itemsForSale(state);
    if (items.isEmpty())
    {
        auto* message = new QLabel(tr("Everything is unlocked!"), rowsParent());
        message->setObjectName(QStringLiteral("panelText"));
        message->setAlignment(Qt::AlignCenter);
        addRow(message);
    }

    for (const ShopItem& item : items)
    {
        const bool affordable = state.applesBalance >= item.info.price;

        auto* row = new ListRow(item.info.art, kSwatchSize, !affordable);
        row->addTitle(item.info.label);
        row->addArtLine({}, QStringLiteral("drumstick"), kPriceArtSize, formatCount(item.info.price));

        auto* buy = new QPushButton(tr("Buy"), row);
        buy->setObjectName(QStringLiteral("buyButton"));
        buy->setFixedSize(kBuyButtonSize);
        buy->setCursor(Qt::PointingHandCursor);
        buy->setEnabled(affordable);
        connect(buy, &QPushButton::clicked, this, [this, item] { emit purchaseRequested(item.kind, item.color); });
        row->setAction(buy);

        addRow(row);
    }
    endRows();
}
