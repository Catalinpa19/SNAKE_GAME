#pragma once

#include <QWidget>

class MenuHeaderWidget;

class MainMenuScreen : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenuScreen(QWidget* parent = nullptr);

    MenuHeaderWidget* header() const { return m_header; }

signals:
    void playRequested();
    void settingsRequested();
    void achievementsRequested();

private:
    MenuHeaderWidget* m_header = nullptr;
};
