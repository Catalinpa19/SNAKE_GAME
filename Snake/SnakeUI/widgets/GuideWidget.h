#pragma once

#include "widgets/ArtImage.h"

#include <QWidget>

// The "press an arrow key or WASD" hint shown over the board before the round starts.
class GuideWidget : public QWidget
{
public:
    explicit GuideWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    ArtImage m_art{QStringLiteral("guide")};
};
