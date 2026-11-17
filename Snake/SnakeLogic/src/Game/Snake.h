#pragma once

#include "Types.h"
#include <vector>

namespace SnakeLogic
{
    class Snake
    {
    private:
        std::vector<Position> body;
        Direction direction;

    public:
        Snake(
            std::vector<Position> initialBody,
            Direction initialDirection
        );

        const std::vector<Position>& getBody() const;
        Position getHead() const;
        Direction getDirection() const;

        bool changeDirection(Direction newDirection);
        void move(bool grow = false);
    };
}