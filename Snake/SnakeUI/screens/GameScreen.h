#pragma once

#include <QColor>
#include <QWidget>

class BoardWidget;
class QFrame;
class QLabel;
class SoundPlayer;

// LOGIC: this screen has no game behind it yet. When it exists, the first direction key starts the round
// (createGame(settings, services)) and every direction key goes to IGame::changeDirection(). QTimer drives
// IGame::tick(), and the score label follows QtGameObserver::scoreChanged.
class GameScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GameScreen(SoundPlayer& sound, QWidget* parent = nullptr);

    QSize minimumSizeHint() const override;

    void reset();
    void setScore(int drumsticks);
    void setGroundColors(const QColor& light, const QColor& dark);

signals:
    void pauseRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QSize panelSizeForCell(int cell) const;

    SoundPlayer& m_sound;
    QFrame* m_panel;
    BoardWidget* m_board;
    QLabel* m_score;
};
