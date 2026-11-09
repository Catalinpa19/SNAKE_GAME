#include "screens/MainMenuScreen.h"

#include "app/Theme.h"
#include "widgets/MenuButton.h"
#include "widgets/MenuHeaderWidget.h"
#include "widgets/TitleWidget.h"

#include <QVBoxLayout>

MainMenuScreen::MainMenuScreen(QWidget* parent)
    : QWidget(parent)
{
    setFixedWidth(Theme::PanelWidth);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(Theme::Spacing);

    layout->addWidget(new TitleWidget(this));

    m_header = new MenuHeaderWidget(this);
    layout->addWidget(m_header);

    auto* play = new MenuButton(tr("Play"), QStringLiteral(":/icons/play.svg"), this);
    auto* settings = new MenuButton(tr("Settings"), QStringLiteral(":/icons/settings.svg"), this);
    auto* achievements = new MenuButton(tr("Achievements"), QStringLiteral(":/icons/star.svg"), this);
    layout->addWidget(play);
    layout->addWidget(settings);
    layout->addWidget(achievements);

    connect(play, &QAbstractButton::clicked, this, &MainMenuScreen::playRequested);
    connect(settings, &QAbstractButton::clicked, this, &MainMenuScreen::settingsRequested);
    connect(achievements, &QAbstractButton::clicked, this, &MainMenuScreen::achievementsRequested);
}

void MainMenuScreen::setScores(int drumsticks, int bestScore)
{
    m_header->setScores(drumsticks, bestScore);
}

void MainMenuScreen::setSnakeColor(const QColor& color)
{
    m_header->setSnakeColor(color);
}

void MainMenuScreen::setGroundColors(const QColor& light, const QColor& dark)
{
    m_header->setGroundColors(light, dark);
}
