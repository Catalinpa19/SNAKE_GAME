#include "ClassicMode.h"

#include <algorithm>
#include <random>

namespace SnakeLogic
{
    void ClassicMode::setupBoard(const GameSettings& settings)
    {
        static_cast<void>(settings);
    }

    void ClassicMode::onTick()
    {
    }

    void ClassicMode::onAppleEaten()
    {
    }

    bool ClassicMode::isCollision(
        Position head,
        const std::vector<Position>& snakeBody,
        int boardWidth,
        int boardHeight
    ) const
    {
        if (head.row < 0 || head.row >= boardHeight ||
            head.column < 0 || head.column >= boardWidth)
        {
            return true;
        }

        if (snakeBody.size() <= 1)
        {
            return false;
        }

        const auto bodyEnd = snakeBody.end() - 1;

        return std::find(snakeBody.begin(), bodyEnd, head) != bodyEnd;
    }

    Position ClassicMode::spawnApple(
        const std::vector<Position>& occupiedPositions,
        int boardWidth,
        int boardHeight
    )
    {
        std::vector<Position> availablePositions;

        for (int row = 0; row < boardHeight; ++row)
        {
            for (int column = 0; column < boardWidth; ++column)
            {
                const Position candidate{ row, column };

                if (std::find(
                    occupiedPositions.begin(),
                    occupiedPositions.end(),
                    candidate) == occupiedPositions.end())
                {
                    availablePositions.push_back(candidate);
                }
            }
        }

        if (availablePositions.empty())
        {
            return { -1, -1 };
        }

        static std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<std::size_t> distribution(
            0,
            availablePositions.size() - 1
        );

        return availablePositions[distribution(generator)];
    }
}