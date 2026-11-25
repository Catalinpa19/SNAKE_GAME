#pragma once

#include "IGameMode.h"

namespace SnakeLogic
{
    class ClassicMode : public IGameMode
    {
    public:
        void setupBoard(const GameSettings& settings) override;
        void onTick() override;
        void onAppleEaten() override;

        bool isCollision(
            Position head,
            const std::vector<Position>& snakeBody,
            int boardWidth,
            int boardHeight
        ) const override;

        Position spawnApple(
            const std::vector<Position>& occupiedPositions,
            int boardWidth,
            int boardHeight
        ) override;
    };
}