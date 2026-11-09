#include "app/MainWindow.h"

#include "app/ProfileStore.h"
#include "app/ScreenNavigator.h"
#include "screens/AchievementsScreen.h"
#include "screens/MainMenuScreen.h"
#include "screens/PlaceholderScreen.h"
#include "screens/SettingsScreen.h"
#include "screens/ShopScreen.h"
#include "widgets/BackgroundWidget.h"
#include "widgets/HudButton.h"

#include <QEvent>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace
{
constexpr int kHudMargin = 16;
} // namespace

MainWindow::MainWindow(ProfileStore& profile, QWidget* parent)
    : QMainWindow(parent)
    , m_profile(profile)
{
    setWindowTitle(tr("Snake"));
    resize(800, 800);

    createScreens();
    createHudButtons();
    connectMenu();
    connectSettings();
    connectShop();

    for (const ColorKind kind : kColorKinds)
        applyColor(kind, m_profile.data().equipped(kind));
    showScores();

    for (PanelScreen* screen : std::initializer_list<PanelScreen*>{m_settings, m_achievements, m_shop, m_game})
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
    m_settings = new SettingsScreen(m_profile.data());
    m_achievements = new AchievementsScreen(m_profile.data());
    m_shop = new ShopScreen(m_profile.data());
    m_game = new PlaceholderScreen(tr("Game"));

    m_navigator->addScreen(Screen::MainMenu, m_menu);
    m_navigator->addScreen(Screen::Settings, m_settings);
    m_navigator->addScreen(Screen::Achievements, m_achievements);
    m_navigator->addScreen(Screen::Shop, m_shop);
    m_navigator->addScreen(Screen::Game, m_game);

    connect(m_navigator, &ScreenNavigator::screenChanged, this, [this](Screen screen) {
        if (screen == Screen::Shop)
            m_shop->refresh(m_profile.data());
        else if (screen == Screen::Achievements)
            m_achievements->refresh(m_profile.data());
    });
}

void MainWindow::createHudButtons()
{
    m_shopButton = new HudButton(QStringLiteral(":/icons/shop.svg"), m_background);
    m_shopButton->setToolTip(tr("Shop"));
    m_soundButton = new HudButton({}, m_background);
    setAudioEnabled(m_profile.data().settings.audioEnabled);

    m_background->installEventFilter(this);
    positionHudButtons();

    connect(m_shopButton, &QAbstractButton::clicked, this, [this] { m_navigator->goTo(Screen::Shop); });
    connect(m_soundButton, &QAbstractButton::clicked, this, [this] {
        updateProfile([](ProfileData& profile) { profile.settings.audioEnabled = !profile.settings.audioEnabled; });
        setAudioEnabled(m_profile.data().settings.audioEnabled);
    });
}

void MainWindow::connectMenu()
{
    // LOGIC: Play starts a round: create it with createGame(settings, services) from Factory.h, show the game
    // screen and drive it with QTimer -> IGame::tick() and key presses -> IGame::changeDirection().
    connect(m_menu, &MainMenuScreen::playRequested, this, [this] { m_navigator->goTo(Screen::Game); });
    connect(m_menu, &MainMenuScreen::settingsRequested, this, [this] { m_navigator->goTo(Screen::Settings); });
    connect(m_menu, &MainMenuScreen::achievementsRequested, this, [this] { m_navigator->goTo(Screen::Achievements); });
}

void MainWindow::connectSettings()
{
    // LOGIC: the selections below build the GameSettings (mode, board, appleCount, speed) that createGame()
    // receives. The colors only affect how the UI draws the snake and the board.
    connect(m_settings, &SettingsScreen::fruitCountSelected, this, [this](int count) {
        updateProfile([count](ProfileData& profile) { profile.settings.appleCount = count; });
    });
    connect(m_settings, &SettingsScreen::speedSelected, this, [this](const QString& speed) {
        updateProfile([&speed](ProfileData& profile) { profile.settings.speed = speed; });
    });
    connect(m_settings, &SettingsScreen::boardSizeSelected, this, [this](const QString& board) {
        updateProfile([&board](ProfileData& profile) { profile.settings.board = board; });
        showScores();
    });
    connect(m_settings, &SettingsScreen::colorSelected, this, [this](ColorKind kind, const QString& id) {
        updateProfile([kind, &id](ProfileData& profile) { profile.equipped(kind) = id; });
        applyColor(kind, id);
    });
}

void MainWindow::connectShop()
{
    // LOGIC: ISkinService::buy(skin) replaces ProfileStore::purchaseColor. The balance and the unlocked
    // skins come back from IProgressService / ISkinService.
    connect(m_shop, &ShopScreen::purchaseRequested, this, [this](ColorKind kind, const QString& id) {
        if (!m_profile.purchaseColor(kind, id))
            return;
        m_profile.save();

        m_shop->refresh(m_profile.data());
        m_settings->unlockColor(kind, id);
        showScores();
    });
}

void MainWindow::onAchievementUnlocked(const QString& id)
{
    // LOGIC: call this from QtGameObserver when IAchievementObserver::onAchievementUnlocked fires. The reward
    // is then granted by IAchievementService, so only the UI refresh below stays.
    if (!m_profile.unlockAchievement(id))
        return;
    m_profile.save();

    const AchievementDef& achievement = Catalog::achievements()[Catalog::achievementIndex(id)];
    if (achievement.rewardColorId != nullptr)
        m_settings->unlockColor(achievement.rewardKind, QLatin1StringView(achievement.rewardColorId));
    m_achievements->refresh(m_profile.data());
    showScores();
}

void MainWindow::updateProfile(const std::function<void(ProfileData&)>& change)
{
    change(m_profile.data());
    m_profile.save();
}

void MainWindow::applyColor(ColorKind kind, const QString& id)
{
    const ColorDef& color = Catalog::colors(kind)[Catalog::colorIndex(kind, id)];
    if (kind == ColorKind::Snake)
        m_menu->setSnakeColor(QColor(color.base));
    else
        m_menu->setGroundColors(QColor(color.base), QColor(color.shade));
}

void MainWindow::showScores()
{
    // LOGIC: IProgressService::applesBalance() and highScore(mode, board). Refresh it after every round.
    m_menu->setScores(m_profile.data().applesBalance, m_profile.data().bestScore());
}

void MainWindow::setAudioEnabled(bool enabled)
{
    // LOGIC: no audio engine yet. When there is one, mute or unmute it here.
    m_soundButton->setIcon(enabled ? QStringLiteral(":/icons/sound_on.svg") : QStringLiteral(":/icons/sound_off.svg"));
    m_soundButton->setToolTip(enabled ? tr("Sound on") : tr("Sound off"));
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_background && event->type() == QEvent::Resize)
        positionHudButtons();
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::positionHudButtons()
{
    m_shopButton->move(kHudMargin, kHudMargin);
    m_soundButton->move(m_background->width() - kHudMargin - m_soundButton->width(), kHudMargin);
    m_shopButton->raise();
    m_soundButton->raise();
}
