#include "widgets/IconButton.h"

#include <QPainter>

namespace
{
constexpr qreal kIdleOpacity = 0.7;
constexpr int kIconSize = 20;
} // namespace

IconButton::IconButton(const QString& iconPath, const QSize& size, QWidget* parent)
    : SvgButton(iconPath, parent)
{
    setFixedSize(size);
}

void IconButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(isDown() || underMouse() ? 1.0 : kIdleOpacity);

    QRectF icon(0, 0, kIconSize, kIconSize);
    icon.moveCenter(QRectF(rect()).center());
    drawIcon(painter, icon);
}
