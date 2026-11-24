#pragma once

#include "screens/ListScreen.h"

struct UiState;

class AchievementsScreen : public ListScreen
{
public:
    explicit AchievementsScreen(const UiState& state, QWidget* parent = nullptr);

    void refresh(const UiState& state);
};
