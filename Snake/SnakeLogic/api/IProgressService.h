#pragma once

#include "Enums.h"
#include "IGameObserver.h"

namespace SnakeLogic
{
    class IProgressService : public IGameObserver
    {
    public:
        virtual ~IProgressService() = default;

        virtual int getApplesBalance() const = 0;
        virtual int getApplesLifetime() const = 0;
        virtual int getGamesPlayed() const = 0;

        virtual int getHighScore(
            GameModeId mode,
            BoardSize board
        ) const = 0;

        virtual bool trySpendApples(int amount) = 0;

        virtual void updateHighScore(
            GameModeId mode,
            BoardSize board,
            int score
        ) = 0;
    };
}