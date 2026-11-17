
#pragma once

namespace SnakeLogic
{
    class Board
    {
    private:
        int width;
        int height;

    public:
        Board(int width, int height);

        int getWidth() const;
        int getHeight() const;

        bool isInside(int row, int column) const;
    };
}
