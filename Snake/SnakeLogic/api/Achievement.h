#pragma once

#include "IAchievementService.h"
#include "ProfileStore.h"

#include <memory>
#include <vector>

namespace SnakeLogic
{
    class AchievementService : public IAchievementService
    {
    private:
        std::shared_ptr<ProfileStore> profile;
        std::vector<IAchievementObserver*> observers;

        void unlock(AchievementId achievement);

    public:
        explicit AchievementService(
            std::shared_ptr<ProfileStore> profile
        );

        std::vector<AchievementInfo> getAchievements() const override;

        void addObserver(IAchievementObserver* observer) override;
        void removeObserver(IAchievementObserver* observer) override;

        void onAppleEaten() override;
        void onScoreChanged(int newScore) override;
        void onGameOver(int finalScore) override;
    };
}