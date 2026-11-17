
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

       
    };
}
