#include "widgets/MenuButton.h"

#include "app/Theme.h"

#include <QPainter>

namespace
{
constexpr int kIconSize = 24;
constexpr int kIconMargin = 12;
constexpr int kFontSize = 16;
} // namespace

MenuButton::MenuButton(const QString& text, const QString& iconPath, QWidget* parent)
    : SvgButton(iconPath, parent)
{
    setText(text);
    setFixedHeight(Theme::ButtonHeight);
}

QSize MenuButton::sizeHint() const
{
    return {Theme::PanelWidth, Theme::ButtonHeight};
}

void MenuButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    drawFrame(painter, isDown() ? Theme::ButtonPressed : underMouse() ? Theme::ButtonHover : Theme::Button, Theme::Ink);

    if (hasIcon())
        drawIcon(painter, QRectF(kIconMargin, (height() - kIconSize) / 2.0, kIconSize, kIconSize));

    QFont font = painter.font();
    font.setPixelSize(kFontSize);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);
    painter.setPen(Theme::Ink);

    const int inset = hasIcon() ? kIconMargin + kIconSize : 0;
    painter.drawText(rect().adjusted(inset, 0, -inset, 0), Qt::AlignCenter, text());
}
