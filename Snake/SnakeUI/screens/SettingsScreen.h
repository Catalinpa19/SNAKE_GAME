#pragma once

#include "app/Catalog.h"
#include "screens/PanelScreen.h"

#include <QList>

struct UiState;
class OptionRow;

class SettingsScreen : public PanelScreen
{
    Q_OBJECT

public:
    explicit SettingsScreen(const UiState& state, QWidget* parent = nullptr);

    // Re-reads which colors are unlocked.
    void refresh(const UiState& state);

signals:
    void fruitCountSelected(int count);
    void speedSelected(const QString& speed);
    void boardSizeSelected(const QString& board);
    void colorSelected(ColorKind kind, int color);

private:
    OptionRow* addColorRow(ColorKind kind, const UiState& state);

    std::array<OptionRow*, kColorKinds.size()> m_colorRows{};
    QList<OptionRow*> m_randomizableRows;
};
