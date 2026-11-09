#pragma once

#include "screens/PanelScreen.h"

// LOGIC: stands in for screens that do not exist yet (the game). Replace it with the real screen.
class PlaceholderScreen : public PanelScreen
{
public:
    explicit PlaceholderScreen(const QString& title, QWidget* parent = nullptr);
};
