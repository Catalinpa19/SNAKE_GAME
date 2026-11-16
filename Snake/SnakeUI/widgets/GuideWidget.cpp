#include "widgets/GuideWidget.h"

#include "app/Theme.h"

#include <QPainter>

namespace
{
constexpr QSize kSize(144, 112);
constexpr int kCornerRadius = 12;
const QColor kBackground(18, 13, 9, 210);
} // namespace

GuideWidget::GuideWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(kSize);
}

void GuideWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(kBackground);
    painter.drawRoundedRect(rect(), kCornerRadius, kCornerRadius);

    m_art.paint(painter, rect().adjusted(16, 14, -16, -14));
}
