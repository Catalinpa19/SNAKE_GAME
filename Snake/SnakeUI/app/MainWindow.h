#pragma once

#include "app/Catalog.h"
#include "app/SoundPlayer.h"
#include "app/UiState.h"

#include <QMainWindow>

class AchievementsScreen;
class BackgroundWidget;
class GameScreen;
class HudButton;
class MainMenuScreen;
class PauseOverlay;
class ScreenNavigator;
class SettingsScreen;
class ShopScreen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void createScreens();
    void createHudButtons();
    void connectMenu();
    void connectSettings();
    void connectShop();
    void connectGame();

    void refreshFromLogic();
    void applyColor(ColorKind kind, int color);
    void showScores();
    void setAudioEnabled(bool enabled);
    void setPaused(bool paused);
    void positionOverlays();

    UiState m_state;
    SoundPlayer m_sound;
    bool m_audioEnabled = true;
    ScreenNavigator* m_navigator = nullptr;
    BackgroundWidget* m_background = nullptr;
    MainMenuScreen* m_menu = nullptr;
    SettingsScreen* m_settings = nullptr;
    AchievementsScreen* m_achievements = nullptr;
    ShopScreen* m_shop = nullptr;
    GameScreen* m_game = nullptr;
    PauseOverlay* m_pause = nullptr;
    HudButton* m_shopButton = nullptr;
    HudButton* m_soundButton = nullptr;
};
