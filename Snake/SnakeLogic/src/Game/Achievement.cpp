#include "AchievementService.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace SnakeLogic
{
    AchievementService::AchievementService(
        std::shared_ptr<ProfileStore> profileStore
    )
        : profile(std::move(profileStore))
    {
        if (!profile)
        {
            throw std::invalid_argument("Profile store cannot be null");
        }
    }

    std::vector<AchievementInfo>
        AchievementService::getAchievements() const
    {
        return {
            {
                AchievementId::FirstApple,
                "First Apple",
                "Eat your first apple",
                profile->isAchievementUnlocked(AchievementId::FirstApple)
            },
            {
                AchievementId::Apples25InRound,
                "25 Apples in One Round",
                "Eat 25 apples in one round",
                profile->isAchievementUnlocked(AchievementId::Apples25InRound)
            },
            {
                AchievementId::Apples100Total,
                "100 Apples Total",
                "Eat 100 apples in total",
                profile->isAchievementUnlocked(AchievementId::Apples100Total)
            },
            {
                AchievementId::GamesPlayed10,
                "10 Games Played",
                "Play 10 games",
                profile->isAchievementUnlocked(AchievementId::GamesPlayed10)
            }
        };
    }

    void AchievementService::addObserver(
        IAchievementObserver* observer
    )
    {
        if (observer == nullptr)
        {
            return;
        }

        const auto it = std::find(
            observers.begin(),
            observers.end(),
            observer
        );

        if (it == observers.end())
        {
            observers.push_back(observer);
        }
    }

    void AchievementService::removeObserver(
        IAchievementObserver* observer
    )
    {
        observers.erase(
            std::remove(
                observers.begin(),
                observers.end(),
                observer
            ),
            observers.end()
        );
    }

    void AchievementService::unlock(AchievementId achievement)
    {
        if (!profile->unlockAchievement(achievement))
        {
            return;
        }

        for (IAchievementObserver* observer : observers)
        {
            observer->onAchievementUnlocked(achievement);
        }
    }

    void AchievementService::onAppleEaten()
    {
        if (profile->getApplesLifetime() >= 1)
        {
            unlock(AchievementId::FirstApple);
        }

        if (profile->getApplesLifetime() >= 100)
        {
            unlock(AchievementId::Apples100Total);
        }
    }

    void AchievementService::onScoreChanged(int newScore)
    {
        if (newScore >= 25)
        {
            unlock(AchievementId::Apples25InRound);
        }
    }

    void AchievementService::onGameOver(int finalScore)
    {
        static_cast<void>(finalScore);

        if (profile->getGamesPlayed() >= 10)
        {
            unlock(AchievementId::GamesPlayed10);
        }
    }
}