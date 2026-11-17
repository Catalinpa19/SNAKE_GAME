#include "Snake.h"

#include <stdexcept>
#include <utility>

namespace SnakeLogic
{
    Snake::Snake(
        std::vector<Position> initialBody,
        Direction initialDirection
    )
        : body(std::move(initialBody)),
        direction(initialDirection)
    {
        if (body.empty())
        {
            throw std::invalid_argument("Snake body cannot be empty");
        }
    }

    const std::vector<Position>& Snake::getBody() const
    {
        return body;
    }

    Position Snake::getHead() const
    {
        return body.front();
    }

    Direction Snake::getDirection() const
    {
        return direction;
    }

    bool Snake::changeDirection(Direction newDirection)
    {
        if (body.size() > 1)
        {
            bool opposite =
                (direction == Direction::Up && newDirection == Direction::Down) ||
                (direction == Direction::Down && newDirection == Direction::Up) ||
                (direction == Direction::Left && newDirection == Direction::Right) ||
                (direction == Direction::Right && newDirection == Direction::Left);

            if (opposite)
            {
                return false;
            }
        }

        direction = newDirection;
        return true;
    }

    void Snake::move(bool grow)
    {
        Position newHead = body.front();

        switch (direction)
        {
        case Direction::Up:
            --newHead.row;
            break;

        case Direction::Down:
            ++newHead.row;
            break;

        case Direction::Left:
            --newHead.column;
            break;

        case Direction::Right:
            ++newHead.column;
            break;
        }

        body.insert(body.begin(), newHead);

        if (!grow)
        {
            body.pop_back();
        }
    }
}