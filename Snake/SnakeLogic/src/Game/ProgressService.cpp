#include "ProgressService.h"

#include <stdexcept>
#include <utility>

namespace SnakeLogic
{
    ProgressService::ProgressService(
        std::shared_ptr<ProfileStore> profileStore
    )
        : profile(std::move(profileStore))
    {
        if (!profile)
        {
            throw std::invalid_argument("Profile store cannot be null");
        }
    }

    int ProgressService::getApplesBalance() const
    {
        return profile->getApplesBalance();
    }

    int ProgressService::getApplesLifetime() const
    {
        return profile->getApplesLifetime();
    }

    int ProgressService::getGamesPlayed() const
    {
        return profile->getGamesPlayed();
    }

    int ProgressService::getHighScore(
        GameModeId mode,
        BoardSize board
    ) const
    {
        return profile->getHighScore(mode, board);
    }

    bool ProgressService::trySpendApples(int amount)
    {
        return profile->trySpendApples(amount);
    }

    void ProgressService::updateHighScore(
        GameModeId mode,
        BoardSize board,
        int score
    )
    {
        profile->updateHighScore(mode, board, score);
    }

    void ProgressService::onAppleEaten()
    {
        profile->addApple();
    }

    void ProgressService::onScoreChanged(int newScore)
    {
        static_cast<void>(newScore);
    }

    void ProgressService::onGameOver(int finalScore)
    {
        static_cast<void>(finalScore);
        profile->incrementGamesPlayed();
    }
}