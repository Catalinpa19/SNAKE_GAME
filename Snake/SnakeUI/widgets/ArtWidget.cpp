#include "widgets/ArtWidget.h"

#include <QPainter>

ArtWidget::ArtWidget(const ArtSpec& art, const QSize& size, QWidget* parent)
    : QWidget(parent)
    , m_art(art)
{
    setFixedSize(size);
}

void ArtWidget::setOpacity(qreal opacity)
{
    m_opacity = opacity;
    update();
}

void ArtWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setOpacity(m_opacity);
    m_art.paint(painter, rect());
}
