
#include "Board.h"

namespace SnakeLogic
{
    Board::Board(int width, int height)
        : width(width), height(height)
    {
    }

    int Board::getWidth() const
    {
        return width;
    }

    int Board::getHeight() const
    {
        return height;
    }

    bool Board::isInside(int row, int column) const
    {
        return row >= 0 &&
            row < height &&
            column >= 0 &&
            column < width;
    }
}
