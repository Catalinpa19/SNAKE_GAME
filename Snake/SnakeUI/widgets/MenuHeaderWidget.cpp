#include "widgets/MenuHeaderWidget.h"

#include "app/Theme.h"

#include <QPainter>
#include <QPainterPath>

namespace
{
constexpr int kGroundTop = 300;
constexpr int kGroundCell = 30;
constexpr int kGroundRows = 2;

const QRectF kSnakeRect(0, 150, 318, 150);
const QRectF kDrumstickRect(48, 34, 64, 64);
const QRectF kTrophyRect(208, 24, 64, 84);
const QRectF kDrumstickScoreRect(40, 114, 80, 34);
const QRectF kTrophyScoreRect(200, 114, 80, 34);
} // namespace

MenuHeaderWidget::MenuHeaderWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(Theme::PanelWidth, Theme::HeaderHeight);
}

void MenuHeaderWidget::setScores(int drumsticks, int bestScore)
{
    m_drumsticks = drumsticks;
    m_bestScore = bestScore;
    update();
}

void MenuHeaderWidget::setSnakeColor(const QColor& color)
{
    m_snake.setReplacements({{QStringLiteral("#4E7CF6"), color.name()},
                             {QStringLiteral("#2B3FA8"), color.darker(160).name()}});
    update();
}

void MenuHeaderWidget::setGroundColors(const QColor& light, const QColor& dark)
{
    m_groundLight = light;
    m_groundDark = dark;
    update();
}

void MenuHeaderWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QPainterPath clip;
    clip.addRoundedRect(rect(), Theme::PanelRadius, Theme::PanelRadius);
    painter.setClipPath(clip);

    painter.fillRect(rect(), Theme::Panel);
    drawGround(painter);
    m_snake.paint(painter, kSnakeRect);
    m_drumstick.paint(painter, kDrumstickRect);
    m_trophy.paint(painter, kTrophyRect);
    drawScores(painter);
}

void MenuHeaderWidget::drawGround(QPainter& painter) const
{
    for (int row = 0; row < kGroundRows; ++row)
    {
        for (int col = 0; col * kGroundCell < width(); ++col)
        {
            const QColor& color = (row + col) % 2 == 0 ? m_groundLight : m_groundDark;
            painter.fillRect(col * kGroundCell, kGroundTop + row * kGroundCell, kGroundCell, kGroundCell, color);
        }
    }
}

void MenuHeaderWidget::drawScores(QPainter& painter) const
{
    QFont font = painter.font();
    font.setPixelSize(26);
    painter.setFont(font);
    painter.setPen(Theme::Cream);
    painter.drawText(kDrumstickScoreRect, Qt::AlignCenter, QString::number(m_drumsticks));
    painter.drawText(kTrophyScoreRect, Qt::AlignCenter, QString::number(m_bestScore));
}
