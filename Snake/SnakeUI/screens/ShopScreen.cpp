#include "screens/ShopScreen.h"

#include "app/ProfileStore.h"
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
    QString id;
    ItemInfo info;
};

QList<ShopItem> itemsForSale(const ProfileData& profile)
{
    QList<ShopItem> items;
    for (const ColorKind kind : kColorKinds)
    {
        for (const ColorDef& color : Catalog::colors(kind))
        {
            const QString id = QLatin1StringView(color.id);
            if (color.source == ColorSource::Shop && !profile.unlocked(kind).contains(id))
                items.append({kind, id, Catalog::colorInfo(kind, id)});
        }
    }
    std::ranges::stable_sort(items, {}, [](const ShopItem& item) { return item.info.price; });
    return items;
}
} // namespace

ShopScreen::ShopScreen(const ProfileData& profile, QWidget* parent)
    : ListScreen(QStringLiteral("drumstick"), kHeaderArtSize, parent)
{
    refresh(profile);
}

void ShopScreen::refresh(const ProfileData& profile)
{
    clearRows();
    setHeaderText(QString::number(profile.applesBalance));

    const QList<ShopItem> items = itemsForSale(profile);
    if (items.isEmpty())
    {
        auto* message = new QLabel(tr("Everything is unlocked!"), rowsParent());
        message->setObjectName(QStringLiteral("panelText"));
        message->setAlignment(Qt::AlignCenter);
        addRow(message);
    }

    for (const ShopItem& item : items)
    {
        const bool affordable = profile.applesBalance >= item.info.price;

        auto* row = new ListRow(item.info.art, kSwatchSize, !affordable);
        row->addTitle(item.info.label);
        row->addArtLine({}, QStringLiteral("drumstick"), kPriceArtSize, QString::number(item.info.price));

        auto* buy = new QPushButton(tr("Buy"), row);
        buy->setObjectName(QStringLiteral("buyButton"));
        buy->setFixedSize(kBuyButtonSize);
        buy->setCursor(Qt::PointingHandCursor);
        buy->setEnabled(affordable);
        connect(buy, &QPushButton::clicked, this, [this, item] { emit purchaseRequested(item.kind, item.id); });
        row->setAction(buy);

        addRow(row);
    }
    endRows();
}
