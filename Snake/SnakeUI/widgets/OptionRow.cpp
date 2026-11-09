#include "widgets/OptionRow.h"

#include "widgets/IconButton.h"
#include "widgets/OptionButton.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QWheelEvent>

namespace
{
constexpr int kCellSpacing = 4;
constexpr int kArrowSpacing = 6;
const QSize kArrowSize(24, 40);
} // namespace

OptionRow::OptionRow(const QList<OptionSpec>& options, const QSize& cellSize, int artBox, int selectedIndex,
                     int visibleCount, QWidget* parent)
    : QWidget(parent)
    , m_options(options)
    , m_group(new QButtonGroup(this))
    , m_cells(new QHBoxLayout)
    , m_visibleCount(visibleCount > 0 ? std::min<int>(visibleCount, options.size()) : options.size())
{
    for (int i = 0; i < options.size(); ++i)
    {
        auto* button = new OptionButton(options[i].art, cellSize, artBox, this);
        button->setChecked(i == selectedIndex);
        m_group->addButton(button, i);
        m_buttons.append(button);
        setLocked(i, options[i].locked);
    }
    connect(m_group, &QButtonGroup::idClicked, this, &OptionRow::selected);

    m_cells->setSpacing(kCellSpacing);
    m_cells->setContentsMargins(0, 0, 0, 0);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(kArrowSpacing);
    layout->addStretch();

    const bool loops = m_visibleCount < options.size();
    if (loops)
    {
        auto* previous = new IconButton(QStringLiteral(":/icons/chevron_left.svg"), kArrowSize, this);
        connect(previous, &QAbstractButton::clicked, this, [this] { rotate(-1); });
        layout->addWidget(previous);
    }
    layout->addLayout(m_cells);
    if (loops)
    {
        auto* next = new IconButton(QStringLiteral(":/icons/chevron_right.svg"), kArrowSize, this);
        connect(next, &QAbstractButton::clicked, this, [this] { rotate(1); });
        layout->addWidget(next);
    }
    layout->addStretch();

    refreshVisibleCells();
    ensureVisible(selectedIndex);
}

void OptionRow::setLocked(int index, bool locked)
{
    if (index < 0 || index >= m_buttons.size())
        return;

    OptionSpec& option = m_options[index];
    option.locked = locked;
    m_buttons[index]->setLocked(locked);

    const QString hint = option.lockedHint.isEmpty() ? tr("locked") : option.lockedHint;
    m_buttons[index]->setToolTip(locked ? tr("%1 (%2)").arg(option.tooltip, hint) : option.tooltip);
}

void OptionRow::select(int index)
{
    if (index < 0 || index >= m_buttons.size() || m_options[index].locked)
        return;

    m_buttons[index]->setChecked(true);
    ensureVisible(index);
    emit selected(index);
}

void OptionRow::selectRandomUnlocked()
{
    QList<int> candidates;
    for (int i = 0; i < m_options.size(); ++i)
    {
        if (!m_options[i].locked)
            candidates.append(i);
    }
    if (!candidates.isEmpty())
        select(candidates[QRandomGenerator::global()->bounded(static_cast<int>(candidates.size()))]);
}

void OptionRow::wheelEvent(QWheelEvent* event)
{
    if (m_visibleCount < m_buttons.size())
        rotate(event->angleDelta().y() > 0 ? -1 : 1);
    event->accept();
}

int OptionRow::wrapped(int index) const
{
    const int count = m_buttons.size();
    return (index % count + count) % count;
}

void OptionRow::rotate(int steps)
{
    m_offset = wrapped(m_offset + steps);
    refreshVisibleCells();
}

void OptionRow::ensureVisible(int index)
{
    if (wrapped(index - m_offset) >= m_visibleCount)
    {
        m_offset = wrapped(index - m_visibleCount / 2);
        refreshVisibleCells();
    }
}

void OptionRow::refreshVisibleCells()
{
    while (m_cells->takeAt(0) != nullptr)
    {
    }
    for (auto* button : m_buttons)
        button->hide();

    for (int i = 0; i < m_visibleCount; ++i)
    {
        auto* button = m_buttons[wrapped(m_offset + i)];
        m_cells->addWidget(button);
        button->show();
    }
}
