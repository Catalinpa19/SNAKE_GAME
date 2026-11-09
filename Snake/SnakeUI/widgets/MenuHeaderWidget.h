#pragma once

#include "app/Theme.h"
#include "widgets/ArtImage.h"

#include <QWidget>

class MenuHeaderWidget : public QWidget
{
public:
    explicit MenuHeaderWidget(QWidget* parent = nullptr);

    void setScores(int drumsticks, int bestScore);
    void setSnakeColor(const QColor& color);
    void setGroundColors(const QColor& light, const QColor& dark);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawGround(QPainter& painter) const;
    void drawScores(QPainter& painter) const;

    ArtImage m_snake{QStringLiteral("snake_menu")};
    ArtImage m_drumstick{QStringLiteral("drumstick")};
    ArtImage m_trophy{QStringLiteral("trophy")};
    QColor m_groundLight = Theme::GroundLight;
    QColor m_groundDark = Theme::GroundDark;
    int m_drumsticks = 0;
    int m_bestScore = 0;
};
