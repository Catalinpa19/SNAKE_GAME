#pragma once

#include "IProgressService.h"
#include "ProfileStore.h"

#include <memory>

namespace SnakeLogic
{
    class ProgressService : public IProgressService
    {
    private:
        std::shared_ptr<ProfileStore> profile;

    public:
        explicit ProgressService(
            std::shared_ptr<ProfileStore> profile
        );

        int getApplesBalance() const override;
        int getApplesLifetime() const override;
        int getGamesPlayed() const override;

        int getHighScore(
            GameModeId mode,
            BoardSize board
        ) const override;

        bool trySpendApples(int amount) override;

        void updateHighScore(
            GameModeId mode,
            BoardSize board,
            int score
        ) override;

        void onAppleEaten() override;
        void onScoreChanged(int newScore) override;
        void onGameOver(int finalScore) override;
    };
}