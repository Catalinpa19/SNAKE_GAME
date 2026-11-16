#pragma once

#include <QColor>
#include <QWidget>

class GuideWidget;
class QVBoxLayout;

// LOGIC: for now an empty checkerboard. It will draw the snake and the drumsticks from the board state
// of IGame, and its column and row count will follow the board size setting. The cells grow with the window.
class BoardWidget : public QWidget
{
public:
    static constexpr int kColumns = 17;
    static constexpr int kRows = 15;
    static constexpr int kMinimumCell = 16;

    explicit BoardWidget(QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void setColors(const QColor& light, const QColor& dark);
    void setGuideVisible(bool visible);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QRect boardRect() const;

    QColor m_light;
    QColor m_dark;
    GuideWidget* m_guide;
    QVBoxLayout* m_overlay;
};
