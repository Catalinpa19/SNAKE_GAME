#include "widgets/SvgButton.h"

#include "app/Theme.h"

#include <QKeyEvent>
#include <QPainter>

SvgButton::SvgButton(const QString& iconPath, QWidget* parent)
    : QAbstractButton(parent)
{
    setIcon(iconPath);
    setAttribute(Qt::WA_Hover);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::TabFocus);
}

void SvgButton::setIcon(const QString& iconPath)
{
    if (!iconPath.isEmpty())
        m_icon.load(iconPath);
    update();
}

void SvgButton::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        click();
    else
        QAbstractButton::keyPressEvent(event);
}

void SvgButton::drawIcon(QPainter& painter, const QRectF& target)
{
    m_icon.render(&painter, target);
}

void SvgButton::drawFrame(QPainter& painter, const QColor& fill, const QColor& focusRing)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(fill);
    painter.drawRoundedRect(QRectF(rect()), Theme::ButtonRadius, Theme::ButtonRadius);

    if (hasFocus())
    {
        painter.setPen(QPen(focusRing, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(QRectF(rect()).adjusted(1, 1, -1, -1), Theme::ButtonRadius - 1, Theme::ButtonRadius - 1);
    }
}
