#include "screens/PauseOverlay.h"

#include "app/SoundPlayer.h"
#include "app/Theme.h"
#include "widgets/MenuButton.h"
#include "widgets/MenuHeaderWidget.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>

namespace
{
const QColor kDim(0, 0, 0, 225);
} // namespace

PauseOverlay::PauseOverlay(SoundPlayer& sound, QWidget* parent)
    : QWidget(parent)
    , m_header(new MenuHeaderWidget(this))
    , m_summary(new QLabel(this))
{
    setFocusPolicy(Qt::StrongFocus);
    hide();

    m_summary->setObjectName(QStringLiteral("panelText"));
    m_summary->setAlignment(Qt::AlignCenter);

    auto* soundTests = new QHBoxLayout;
    soundTests->setSpacing(Theme::Spacing);
    const auto addSoundTest = [&](const QString& text, Sound effect) {
        auto* button = new MenuButton(text, {}, this);
        connect(button, &QAbstractButton::clicked, this, [&sound, effect] { sound.play(effect); });
        soundTests->addWidget(button);
    };
    addSoundTest(tr("Win"), Sound::Win);
    addSoundTest(tr("Lose"), Sound::Hurt);
    addSoundTest(tr("Eat"), Sound::Eat);

    auto* resume = new MenuButton(tr("Resume"), QStringLiteral(":/icons/play.svg"), this);
    auto* leave = new MenuButton(tr("Leave game"), QStringLiteral(":/icons/back.svg"), this);
    connect(resume, &QAbstractButton::clicked, this, &PauseOverlay::resumeRequested);
    connect(leave, &QAbstractButton::clicked, this, &PauseOverlay::leaveRequested);

    auto* content = new QWidget(this);
    content->setFixedWidth(Theme::PanelWidth);
    auto* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(Theme::Spacing);
    contentLayout->addWidget(m_header);
    contentLayout->addWidget(m_summary);
    contentLayout->addLayout(soundTests);
    contentLayout->addWidget(resume);
    contentLayout->addWidget(leave);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(content, 0, Qt::AlignCenter);
}

void PauseOverlay::setSummary(const QString& mode, const QString& speed, const QString& board)
{
    m_summary->setText(tr("%1 mode  •  %2 speed  •  %3 board").arg(mode, speed, board));
}

void PauseOverlay::paintEvent(QPaintEvent*)
{
    QPainter(this).fillRect(rect(), kDim);
}

void PauseOverlay::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        emit resumeRequested();
    else
        QWidget::keyPressEvent(event);
}

void PauseOverlay::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    setFocus();
}
