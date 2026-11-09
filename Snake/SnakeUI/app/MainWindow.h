#pragma once

#include "app/Catalog.h"

#include <QMainWindow>

#include <functional>

class AchievementsScreen;
class BackgroundWidget;
class HudButton;
class MainMenuScreen;
class PanelScreen;
struct ProfileData;
class ProfileStore;
class ScreenNavigator;
class SettingsScreen;
class ShopScreen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ProfileStore& profile, QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void createScreens();
    void createHudButtons();
    void connectMenu();
    void connectSettings();
    void connectShop();

    void onAchievementUnlocked(const QString& id);
    void updateProfile(const std::function<void(ProfileData&)>& change);
    void applyColor(ColorKind kind, const QString& id);
    void showScores();
    void setAudioEnabled(bool enabled);
    void positionHudButtons();

    ProfileStore& m_profile;
    ScreenNavigator* m_navigator = nullptr;
    BackgroundWidget* m_background = nullptr;
    MainMenuScreen* m_menu = nullptr;
    SettingsScreen* m_settings = nullptr;
    AchievementsScreen* m_achievements = nullptr;
    ShopScreen* m_shop = nullptr;
    PanelScreen* m_game = nullptr;
    HudButton* m_shopButton = nullptr;
    HudButton* m_soundButton = nullptr;
};
