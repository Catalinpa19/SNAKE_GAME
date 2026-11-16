#include "screens/GameScreen.h"

#include "app/SoundPlayer.h"
#include "app/Theme.h"
#include "widgets/ArtWidget.h"
#include "widgets/BoardWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>

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
    , m_board(new BoardWidget(this))
    , m_score(new QLabel(this))
{
    setFocusPolicy(Qt::StrongFocus);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* panel = new QFrame(this);
    panel->setObjectName(QStringLiteral("panel"));
    auto* content = new QVBoxLayout(panel);
    content->setContentsMargins(Theme::PanelPadding, Theme::PanelPadding, Theme::PanelPadding, Theme::PanelPadding);
    content->setSpacing(Theme::Spacing);

    auto* topBar = new QHBoxLayout;
    topBar->addWidget(new ArtWidget(QStringLiteral("drumstick"), kScoreIconSize, panel));
    m_score->setObjectName(QStringLiteral("panelHeader"));
    topBar->addWidget(m_score);
    topBar->addStretch();
    content->addLayout(topBar);
    content->addWidget(m_board);

    layout->addWidget(panel);
    reset();
}

void GameScreen::reset()
{
    m_score->setText(QStringLiteral("0"));
    m_board->setGuideVisible(true);
    setFocus();
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
