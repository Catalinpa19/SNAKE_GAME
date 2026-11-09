#pragma once

#include <QColor>
#include <QWidget>

class MenuHeaderWidget;

class MainMenuScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuScreen(QWidget* parent = nullptr);

    void setScores(int drumsticks, int bestScore);
    void setSnakeColor(const QColor& color);
    void setGroundColors(const QColor& light, const QColor& dark);

signals:
    void playRequested();
    void settingsRequested();
    void achievementsRequested();

private:
    MenuHeaderWidget* m_header = nullptr;
};
