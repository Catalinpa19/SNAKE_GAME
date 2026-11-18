#include "screens/SettingsScreen.h"

#include "app/NumberFormat.h"
#include "app/UiState.h"
#include "app/Theme.h"
#include "widgets/MenuButton.h"
#include "widgets/OptionRow.h"

namespace
{
constexpr QSize kLargeCell(68, 68);
constexpr QSize kColorCell(52, 52);
constexpr int kVisibleColors = 5;
} // namespace

SettingsScreen::SettingsScreen(const UiState& state, QWidget* parent)
    : PanelScreen(Theme::SettingsWidth, parent)
{
    addSection(new OptionRow({{QStringLiteral("drumstick"), tr("Drumstick")}}, kLargeCell, 48, 0, 0, this));
    addSection(new OptionRow({{QStringLiteral("trophy"), tr("Classic")}}, kLargeCell, 64, 0, 0, this));

    QList<OptionSpec> counts;
    for (const int count : Catalog::kFruitCounts)
    {
        counts.append({QStringLiteral("count_%1").arg(count),
                       count == 1 ? tr("1 drumstick") : tr("%1 drumsticks").arg(formatCount(count))});
    }
    auto* countRow = new OptionRow(counts, kLargeCell, 64, Catalog::fruitCountIndex(state.settings.appleCount), 0, this);
    connect(countRow, &OptionRow::selected, this, [this](int i) { emit fruitCountSelected(Catalog::kFruitCounts[i]); });
    addSection(countRow);

    const QList<OptionSpec> speeds{
        {QStringLiteral("speed_slow"), tr("Slow")},
        {QStringLiteral("speed_normal"), tr("Normal")},
        {QStringLiteral("speed_fast"), tr("Fast")},
    };
    auto* speedRow = new OptionRow(speeds, kLargeCell, 48, Catalog::speedIndex(state.settings.speed), 0, this);
    connect(speedRow, &OptionRow::selected, this, [this](int i) { emit speedSelected(QLatin1StringView(Catalog::kSpeeds[i])); });
    addSection(speedRow);

    const QList<OptionSpec> sizes{
        {QStringLiteral("size_2x2"), tr("Small")},
        {QStringLiteral("size_3x3"), tr("Normal")},
        {QStringLiteral("size_4x4"), tr("Large")},
    };
    auto* sizeRow = new OptionRow(sizes, kLargeCell, 48, Catalog::boardSizeIndex(state.settings.board), 0, this);
    connect(sizeRow, &OptionRow::selected, this, [this](int i) { emit boardSizeSelected(QLatin1StringView(Catalog::kBoardSizes[i])); });
    addSection(sizeRow);

    m_randomizableRows = {countRow, speedRow, sizeRow};
    for (const ColorKind kind : kColorKinds)
        m_randomizableRows.append(addColorRow(kind, state));

    connect(addButton(tr("Randomize"), QStringLiteral(":/icons/dice.svg")), &QAbstractButton::clicked, this, [this] {
        for (OptionRow* row : m_randomizableRows)
            row->selectRandomUnlocked();
    });
    addBackButton();
}

OptionRow* SettingsScreen::addColorRow(ColorKind kind, const UiState& state)
{
    QList<OptionSpec> options;
    const auto colors = Catalog::colors(kind);
    for (int i = 0; i < static_cast<int>(colors.size()); ++i)
    {
        const ItemInfo info = Catalog::colorInfo(kind, i);
        const QString hint = info.source == ColorSource::Shop ? tr("buy it in the Shop") : tr("unlock it with an achievement");
        options.append({info.art, info.label, !state.unlocked(kind).contains(i), hint});
    }

    const int artBox = kind == ColorKind::Snake ? 50 : 48;
    auto* row = new OptionRow(options, kColorCell, artBox, state.equipped(kind), kVisibleColors, this);
    connect(row, &OptionRow::selected, this, [this, kind](int color) { emit colorSelected(kind, color); });

    m_colorRows[static_cast<size_t>(kind)] = row;
    addSection(row);
    return row;
}

void SettingsScreen::refresh(const UiState& state)
{
    for (const ColorKind kind : kColorKinds)
    {
        OptionRow* row = m_colorRows[static_cast<size_t>(kind)];
        const auto colors = Catalog::colors(kind);
        for (int i = 0; i < static_cast<int>(colors.size()); ++i)
            row->setLocked(i, !state.unlocked(kind).contains(i));
    }
}