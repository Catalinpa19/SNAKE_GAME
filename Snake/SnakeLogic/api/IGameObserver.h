
#pragma once

namespace SnakeLogic
{
    class IGameObserver
    {
    public:
        virtual ~IGameObserver() = default;

        virtual void onAppleEaten() = 0;

        virtual void onScoreChanged(int newScore) = 0;

        virtual void onGameOver(int finalScore) = 0;
    };
}
