#pragma once

#include <QAbstractButton>
#include <QtSvg/QSvgRenderer>

// Base of the icon buttons: loads the SVG icon, shows the hand cursor and activates on Enter.
class SvgButton : public QAbstractButton
{
public:
    void setIcon(const QString& iconPath);

protected:
    explicit SvgButton(const QString& iconPath, QWidget* parent);

    void keyPressEvent(QKeyEvent* event) override;

    bool hasIcon() const { return m_icon.isValid(); }
    void drawIcon(QPainter& painter, const QRectF& target);
    void drawFrame(QPainter& painter, const QColor& fill, const QColor& focusRing);

private:
    QSvgRenderer m_icon;
};
