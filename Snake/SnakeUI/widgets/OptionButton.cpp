#include "widgets/OptionButton.h"

#include "app/Theme.h"

#include <QPainter>

namespace
{
constexpr int kCornerRadius = 6;
constexpr int kLockSize = 20;
constexpr qreal kIdleOpacity = 0.55;
} // namespace

OptionButton::OptionButton(const ArtSpec& art, const QSize& cellSize, int artBox, QWidget* parent)
    : QAbstractButton(parent)
    , m_art(art)
    , m_cellSize(cellSize)
    , m_artBox(artBox)
{
    setCheckable(true);
    setAttribute(Qt::WA_Hover);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::TabFocus);
    setFixedSize(cellSize);
}

QSize OptionButton::sizeHint() const
{
    return m_cellSize;
}

void OptionButton::setLocked(bool locked)
{
    m_locked = locked;
    setEnabled(!locked);
    setCursor(locked ? Qt::ArrowCursor : Qt::PointingHandCursor);
    update();
}

void OptionButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const QRectF frame = QRectF(rect()).adjusted(1, 1, -1, -1);
    if (isChecked())
    {
        painter.setPen(QPen(Theme::GroundLight, 2));
        painter.setBrush(Theme::Selection);
        painter.drawRoundedRect(frame, kCornerRadius, kCornerRadius);
    }
    else if (underMouse() && !m_locked)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Theme::Hover);
        painter.drawRoundedRect(frame, kCornerRadius, kCornerRadius);
    }

    if (m_locked)
        painter.setOpacity(Theme::DimmedOpacity);
    else
        painter.setOpacity(isChecked() || underMouse() ? 1.0 : kIdleOpacity);

    QRectF art(0, 0, m_artBox, m_artBox);
    art.moveCenter(QRectF(rect()).center());
    m_art.paint(painter, art);

    if (m_locked)
    {
        painter.setOpacity(1.0);
        QRectF lock(0, 0, kLockSize, kLockSize);
        lock.moveCenter(QRectF(rect()).center());
        m_lockIcon.render(&painter, lock);
    }
}
