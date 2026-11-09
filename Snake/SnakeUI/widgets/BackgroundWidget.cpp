#include "widgets/BackgroundWidget.h"

#include <QPainter>

BackgroundWidget::BackgroundWidget(QWidget* parent)
    : QWidget(parent)
{
}

void BackgroundWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect target(QPoint(), m_image.size().scaled(size(), Qt::KeepAspectRatioByExpanding));
    target.moveCenter(rect().center());
    painter.drawPixmap(target, m_image);
}
