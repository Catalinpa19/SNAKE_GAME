
#pragma once

#include "Enums.h"
#include "Types.h"
#include "IGameObserver.h"

#include <vector>

namespace SnakeLogic
{
    class IAchievementObserver
    {
    public:
        virtual ~IAchievementObserver() = default;

        virtual void onAchievementUnlocked(
            AchievementId achievement
        ) = 0;
    };

    class IAchievementService : public IGameObserver
    {
    public:
        virtual ~IAchievementService() = default;

        virtual std::vector<AchievementInfo> getAchievements() const = 0;

        virtual void addObserver(
            IAchievementObserver* observer
        ) = 0;

        virtual void removeObserver(
            IAchievementObserver* observer
        ) = 0;
    };
}
