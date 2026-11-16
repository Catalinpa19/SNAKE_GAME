#include "screens/GameScreen.h"

#include "app/NumberFormat.h"
#include "app/SoundPlayer.h"
#include "app/Theme.h"
#include "widgets/ArtWidget.h"
#include "widgets/BoardWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>

#include <algorithm>
#include <optional>

namespace
{
constexpr QSize kScoreIconSize(32, 32);

std::optional<Sound> directionSound(int key)
{
    switch (key)
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        return Sound::Up;
    case Qt::Key_S:
    case Qt::Key_Down:
        return Sound::Down;
    case Qt::Key_A:
    case Qt::Key_Left:
        return Sound::Left;
    case Qt::Key_D:
    case Qt::Key_Right:
        return Sound::Right;
    default:
        return std::nullopt;
    }
}
} // namespace

GameScreen::GameScreen(SoundPlayer& sound, QWidget* parent)
    : QWidget(parent)
    , m_sound(sound)
    , m_panel(new QFrame(this))
    , m_board(new BoardWidget(m_panel))
    , m_score(new QLabel(m_panel))
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_panel->setObjectName(QStringLiteral("panel"));
    auto* content = new QVBoxLayout(m_panel);
    content->setContentsMargins(Theme::PanelPadding, Theme::PanelPadding, Theme::PanelPadding, Theme::PanelPadding);
    content->setSpacing(Theme::Spacing);

    auto* topBar = new QHBoxLayout;
    topBar->addWidget(new ArtWidget(QStringLiteral("drumstick"), kScoreIconSize, m_panel));
    m_score->setObjectName(QStringLiteral("panelHeader"));
    topBar->addWidget(m_score);
    topBar->addStretch();
    content->addLayout(topBar);
    content->addWidget(m_board);

    reset();
}

QSize GameScreen::minimumSizeHint() const
{
    return panelSizeForCell(BoardWidget::kMinimumCell);
}

QSize GameScreen::panelSizeForCell(int cell) const
{
    const int padding = 2 * Theme::PanelPadding;
    return {padding + BoardWidget::kColumns * cell,
            padding + kScoreIconSize.height() + Theme::Spacing + BoardWidget::kRows * cell};
}

void GameScreen::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    const QSize overhead = panelSizeForCell(0);
    const int cell = std::max(BoardWidget::kMinimumCell,
                              std::min((width() - overhead.width()) / BoardWidget::kColumns,
                                       (height() - overhead.height()) / BoardWidget::kRows));
    QRect panel(QPoint(), panelSizeForCell(cell));
    panel.moveCenter(rect().center());
    m_panel->setGeometry(panel);
}
void GameScreen::reset()
{
    setScore(0);
    m_board->setGuideVisible(true);
    setFocus();
}

void GameScreen::setScore(int drumsticks)
{
    m_score->setText(formatCount(drumsticks));
}

void GameScreen::setGroundColors(const QColor& light, const QColor& dark)
{
    m_board->setColors(light, dark);
}

void GameScreen::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        emit pauseRequested();
        return;
    }

    const std::optional<Sound> sound = directionSound(event->key());
    if (!sound || event->isAutoRepeat())
    {
        QWidget::keyPressEvent(event);
        return;
    }

    m_sound.play(*sound);
    m_board->setGuideVisible(false);
}
