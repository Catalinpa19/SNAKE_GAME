#include "widgets/TitleWidget.h"

#include "app/Theme.h"

#include <QPainter>

namespace
{
constexpr int kHeight = 112;
const QRectF kTitleRect(20, 6, 280, 80);
const QRectF kSubtitleRect(0, 92, Theme::PanelWidth, 16);
} // namespace

TitleWidget::TitleWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(Theme::PanelWidth, kHeight);
    setAccessibleName(tr("Snake by los pollos hermanos"));
}

void TitleWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    m_title.paint(painter, kTitleRect);
    m_subtitle.paint(painter, kSubtitleRect);
}
