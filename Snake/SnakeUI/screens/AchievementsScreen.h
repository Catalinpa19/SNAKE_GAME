#pragma once

#include "screens/ListScreen.h"

struct ProfileData;

class AchievementsScreen : public ListScreen
{
public:
    explicit AchievementsScreen(const ProfileData& profile, QWidget* parent = nullptr);

    void refresh(const ProfileData& profile);
};
