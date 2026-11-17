
#pragma once

namespace SnakeLogic
{
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right
    };

    enum class GameState
    {
        Ready,
        Running,
        Paused,
        GameOver
    };

    enum class GameModeId
    {
        Classic
    };

    enum class BoardSize
    {
        Small,
        Medium,
        Large
    };

    enum class GameSpeed
    {
        Slow,
        Normal,
        Fast
    };


    enum class SkinId
    {
        Default,
        Neon
    };

    enum class AchievementId
    {
        FirstApple,
        Apples25InRound,
        Apples100Total,
        GamesPlayed10
    };

}
