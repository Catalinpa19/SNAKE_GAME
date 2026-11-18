#include "app/MainWindow.h"

#include "app/ScreenNavigator.h"
#include "screens/AchievementsScreen.h"
#include "screens/GameScreen.h"
#include "screens/MainMenuScreen.h"
#include "screens/PauseOverlay.h"
#include "screens/SettingsScreen.h"
#include "screens/ShopScreen.h"
#include "widgets/BackgroundWidget.h"
#include "widgets/HudButton.h"
#include "widgets/MenuHeaderWidget.h"

#include <QEvent>
#include <QKeyEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace
{
constexpr int kHudMargin = 16;
const QSize kMinimumWindowSize(720, 780);
} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Snake"));
    resize(800, 800);
    setMinimumSize(kMinimumWindowSize);

    createScreens();
    createHudButtons();
    connectMenu();
    connectSettings();
    connectShop();
    connectGame();

    for (const ColorKind kind : kColorKinds)
        applyColor(kind, m_state.equipped(kind));
    showScores();

    for (PanelScreen* screen : std::initializer_list<PanelScreen*>{m_settings, m_achievements, m_shop})
        connect(screen, &PanelScreen::backRequested, this, [this] { m_navigator->goTo(Screen::MainMenu); });

    m_navigator->goTo(Screen::MainMenu);
}

void MainWindow::createScreens()
{
    m_background = new BackgroundWidget(this);
    auto* layout = new QVBoxLayout(m_background);
    layout->setContentsMargins(0, 0, 0, 0);
    auto* stack = new QStackedWidget(m_background);
    layout->addWidget(stack);
    setCentralWidget(m_background);

    m_navigator = new ScreenNavigator(stack, this);
    m_menu = new MainMenuScreen;
    m_settings = new SettingsScreen(m_state);
    m_achievements = new AchievementsScreen(m_state);
    m_shop = new ShopScreen(m_state);
    m_game = new GameScreen(m_sound);
    m_pause = new PauseOverlay(m_sound, m_background);

    m_navigator->addScreen(Screen::MainMenu, m_menu);
    m_navigator->addScreen(Screen::Settings, m_settings);
    m_navigator->addScreen(Screen::Achievements, m_achievements);
    m_navigator->addScreen(Screen::Shop, m_shop);
    m_navigator->addScreen(Screen::Game, m_game, true);

    connect(m_navigator, &ScreenNavigator::screenChanged, this, [this](Screen screen) {
        m_shopButton->setVisible(screen != Screen::Game);
        if (screen == Screen::Game)
            m_game->reset();
        else
            refreshFromLogic();
    });
}

void MainWindow::createHudButtons()
{
    m_shopButton = new HudButton(QStringLiteral(":/icons/shop.svg"), m_background);
    m_shopButton->setToolTip(tr("Shop"));
    m_soundButton = new HudButton({}, m_background);
    setAudioEnabled(m_audioEnabled);

    m_background->installEventFilter(this);
    positionOverlays();

    connect(m_shopButton, &QAbstractButton::clicked, this, [this] { m_navigator->goTo(Screen::Shop); });
    connect(m_soundButton, &QAbstractButton::clicked, this, [this] { setAudioEnabled(!m_audioEnabled); });
}

void MainWindow::connectMenu()
{
    connect(m_menu, &MainMenuScreen::playRequested, this, [this] { m_navigator->goTo(Screen::Game); });
    connect(m_menu, &MainMenuScreen::settingsRequested, this, [this] { m_navigator->goTo(Screen::Settings); });
    connect(m_menu, &MainMenuScreen::achievementsRequested, this, [this] { m_navigator->goTo(Screen::Achievements); });
}

void MainWindow::connectSettings()
{
    // LOGIC: persisting the picks and turning them into the GameSettings belongs to SnakeLogic.
    connect(m_settings, &SettingsScreen::fruitCountSelected, this, [this](int count) { m_state.settings.appleCount = count; });
    connect(m_settings, &SettingsScreen::speedSelected, this, [this](const QString& speed) { m_state.settings.speed = speed; });
    connect(m_settings, &SettingsScreen::boardSizeSelected, this, [this](const QString& board) {
        m_state.settings.board = board;
        // LOGIC: the high score depends on the board size: IProgressService::highScore(mode, board).
        showScores();
    });
    connect(m_settings, &SettingsScreen::colorSelected, this, [this](ColorKind kind, int color) {
        m_state.equipped(kind) = color;
        applyColor(kind, color);
    });
}

void MainWindow::connectShop()
{
    connect(m_shop, &ShopScreen::purchaseRequested, this, [](ColorKind, int) {
        // LOGIC: ISkinService::buy(skin). It checks the price, takes the drumsticks and unlocks the skin.
        // Then call refreshFromLogic().
    });
}

void MainWindow::connectGame()
{
    // LOGIC: Resume and Leave game map to pausing and ending the IGame round.
    connect(m_game, &GameScreen::pauseRequested, this, [this] { setPaused(true); });
    connect(m_pause, &PauseOverlay::resumeRequested, this, [this] { setPaused(false); });
    connect(m_pause, &PauseOverlay::leaveRequested, this, [this] {
        setPaused(false);
        m_navigator->goTo(Screen::MainMenu);
    });

    // LOGIC: connect QtGameObserver here: achievementUnlocked(id) -> refreshFromLogic(), and gameOver
    // -> refreshFromLogic() (new balance and high score).
}

void MainWindow::refreshFromLogic()
{
    // LOGIC: read the balance, high score, unlocked skins and unlocked achievements from IProgressService,
    // ISkinService and IAchievementService into m_state. For now m_state only holds placeholders.
    m_shop->refresh(m_state);
    m_achievements->refresh(m_state);
    m_settings->refresh(m_state);
    showScores();
}

void MainWindow::applyColor(ColorKind kind, int colorId)
{
    const ColorDef& color = Catalog::colors(kind)[colorId];
    for (MenuHeaderWidget* header : {m_menu->header(), m_pause->header()})
    {
        if (kind == ColorKind::Snake)
            header->setSnakeColor(QColor(color.base));
        else
            header->setGroundColors(QColor(color.base), QColor(color.shade));
    }
    if (kind == ColorKind::Map)
        m_game->setGroundColors(QColor(color.base), QColor(color.shade));
}

void MainWindow::showScores()
{
    for (MenuHeaderWidget* header : {m_menu->header(), m_pause->header()})
        header->setScores(m_state.applesBalance, m_state.bestScore);
}

void MainWindow::setAudioEnabled(bool enabled)
{
    m_audioEnabled = enabled;
    m_sound.setEnabled(enabled);
    m_soundButton->setIcon(enabled ? QStringLiteral(":/icons/sound_on.svg") : QStringLiteral(":/icons/sound_off.svg"));
    m_soundButton->setToolTip(enabled ? tr("Sound on") : tr("Sound off"));
}

void MainWindow::setPaused(bool paused)
{
    if (paused)
    {
        m_pause->setSummary(m_state.settings.mode, m_state.settings.speed, m_state.settings.board);
        m_pause->raise();
        m_soundButton->raise();
        m_pause->show();
    }
    else
    {
        m_pause->hide();
        m_game->setFocus();
    }
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_background && event->type() == QEvent::Resize)
        positionOverlays();
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    const Screen screen = m_navigator->current();
    const bool leavesWithEscape = screen == Screen::Settings || screen == Screen::Achievements || screen == Screen::Shop;
    if (event->key() == Qt::Key_Escape && leavesWithEscape)
        m_navigator->goTo(Screen::MainMenu);
    else
        QMainWindow::keyPressEvent(event);
}

void MainWindow::positionOverlays()
{
    m_pause->setGeometry(m_background->rect());
    m_shopButton->move(kHudMargin, kHudMargin);
    m_soundButton->move(m_background->width() - kHudMargin - m_soundButton->width(), kHudMargin);
    m_shopButton->raise();
    if (m_pause->isVisible())
        m_pause->raise();
    m_soundButton->raise();
}
