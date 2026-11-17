
#pragma once

#include "Enums.h"
#include "Types.h"
#include "IGameObserver.h"

namespace SnakeLogic
{
    class IGame
    {
    public:
        virtual ~IGame() = default;

        virtual void changeDirection(Direction direction) = 0;

        virtual void tick() = 0;

        virtual void start() = 0;

        virtual void pause() = 0;

        virtual void resume() = 0;

        virtual void addObserver(IGameObserver* observer) = 0;

        virtual void removeObserver(IGameObserver* observer) = 0;
    };
}
