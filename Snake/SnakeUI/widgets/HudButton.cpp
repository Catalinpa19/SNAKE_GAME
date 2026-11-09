#include "widgets/HudButton.h"

#include "app/Theme.h"

#include <QPainter>

namespace
{
constexpr int kSize = 44;
constexpr int kIconSize = 26;
} // namespace

HudButton::HudButton(const QString& iconPath, QWidget* parent)
    : SvgButton(iconPath, parent)
{
    setFixedSize(kSize, kSize);
}

void HudButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    drawFrame(painter, isDown() ? Theme::HudPressed : underMouse() ? Theme::HudHover : Theme::Panel, Theme::Gold);

    QRectF icon(0, 0, kIconSize, kIconSize);
    icon.moveCenter(QRectF(rect()).center());
    drawIcon(painter, icon);
}
