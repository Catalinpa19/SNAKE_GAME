#pragma once

#include <QWidget>

class MenuButton;
class QVBoxLayout;

// Base of the screens made of a dark panel with buttons under it. Every such screen can go back.
class PanelScreen : public QWidget
{
    Q_OBJECT

public:
    explicit PanelScreen(int width, QWidget* parent = nullptr);

signals:
    void backRequested();

protected:
    QVBoxLayout* panelLayout() const { return m_panelLayout; }

    // Adds the widget to the panel, separated from the previous one by a divider.
    void addSection(QWidget* section);
    MenuButton* addButton(const QString& text, const QString& iconPath);
    void addBackButton();

private:
    QVBoxLayout* m_screenLayout;
    QVBoxLayout* m_panelLayout;
};
