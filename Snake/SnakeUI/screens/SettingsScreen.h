#pragma once

#include "app/Catalog.h"
#include "screens/PanelScreen.h"

#include <QList>

struct ProfileData;
class OptionRow;

class SettingsScreen : public PanelScreen
{
    Q_OBJECT

public:
    explicit SettingsScreen(const ProfileData& profile, QWidget* parent = nullptr);

    void unlockColor(ColorKind kind, const QString& id);

signals:
    void fruitCountSelected(int count);
    void speedSelected(const QString& speed);
    void boardSizeSelected(const QString& board);
    void colorSelected(ColorKind kind, const QString& id);

private:
    OptionRow* addColorRow(ColorKind kind, const ProfileData& profile);

    std::array<OptionRow*, kColorKinds.size()> m_colorRows{};
    QList<OptionRow*> m_randomizableRows;
};
