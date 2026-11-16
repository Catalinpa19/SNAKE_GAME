#pragma once

#include <QWidget>

class MenuHeaderWidget;
class QLabel;
class SoundPlayer;

// Dims the whole window and shows the round info with Resume / Leave game. The Win, Lose and Eat buttons
// only play the sounds, so they can be checked before the game exists.
class PauseOverlay : public QWidget
{
    Q_OBJECT

public:
    PauseOverlay(SoundPlayer& sound, QWidget* parent = nullptr);

    MenuHeaderWidget* header() const { return m_header; }
    void setSummary(const QString& mode, const QString& speed, const QString& board);

signals:
    void resumeRequested();
    void leaveRequested();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    MenuHeaderWidget* m_header;
    QLabel* m_summary;
};
