#pragma once

#include "widgets/ArtImage.h"

#include <QWidget>

class ArtWidget : public QWidget
{
public:
    ArtWidget(const ArtSpec& art, const QSize& size, QWidget* parent = nullptr);

    void setOpacity(qreal opacity);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ArtImage m_art;
    qreal m_opacity = 1.0;
};
