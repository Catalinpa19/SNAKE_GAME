#include "widgets/TitleWidget.h"

#include "app/Theme.h"

#include <QPainter>

namespace
{
constexpr int kHeight = 144;
constexpr int kShadowOffset = 5;
const QRectF kTitleRect(0, 0, Theme::PanelWidth, 112);
const QRectF kSubtitleRect(0, 112, Theme::PanelWidth, 26);
} // namespace

TitleWidget::TitleWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(Theme::PanelWidth, kHeight);
}

void TitleWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont titleFont(QStringLiteral("Impact"));
    titleFont.setPixelSize(104);
    painter.setFont(titleFont);
    painter.setPen(Theme::TitleShadow);
    painter.drawText(kTitleRect.translated(kShadowOffset, kShadowOffset), Qt::AlignCenter, tr("Snake"));
    painter.setPen(Theme::GroundLight);
    painter.drawText(kTitleRect, Qt::AlignCenter, tr("Snake"));

    QFont subtitleFont = font();
    subtitleFont.setPixelSize(15);
    subtitleFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    painter.setFont(subtitleFont);
    painter.setPen(Theme::GroundLight);
    painter.drawText(kSubtitleRect, Qt::AlignCenter, tr("by los pollos hermanos"));
}
