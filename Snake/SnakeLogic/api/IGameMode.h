#pragma once

#include "Types.h"

#include <vector>

namespace SnakeLogic
{
    class IGameMode
    {
    public:
        virtual ~IGameMode() = default;

        virtual void setupBoard(const GameSettings& settings) = 0;

        virtual void onTick() = 0;

        virtual void onAppleEaten() = 0;

        virtual bool isCollision(
            Position head,
            const std::vector<Position>& snakeBody,
            int boardWidth,
            int boardHeight
        ) const = 0;

        virtual Position spawnApple(
            const std::vector<Position>& occupiedPositions,
            int boardWidth,
            int boardHeight
        ) = 0;
    };
}