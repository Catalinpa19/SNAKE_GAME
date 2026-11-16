#pragma once

#include <QObject>
#include <map>

class QStackedWidget;
class QWidget;

enum class Screen
{
    MainMenu,
    Settings,
    Achievements,
    Shop,
    Game
};

class ScreenNavigator : public QObject
{
    Q_OBJECT

public:
    explicit ScreenNavigator(QStackedWidget* stack, QObject* parent = nullptr);

    // Each screen is centered in its own page, so it stays centered when the window is resized.
    // A screen that fills its page stretches with the window instead of staying centered.
    void addScreen(Screen id, QWidget* screen, bool fillsPage = false);
    Screen current() const { return m_current; }

public slots:
    void goTo(Screen id);

signals:
    void screenChanged(Screen id);

private:
    QStackedWidget* m_stack;
    std::map<Screen, QWidget*> m_pages;
    Screen m_current = Screen::MainMenu;
};
