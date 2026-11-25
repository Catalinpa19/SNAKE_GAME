#pragma once

#include "IGame.h"
#include "IGameMode.h"
#include "Board.h"
#include "Snake.h"

#include <memory>
#include <vector>

namespace SnakeLogic
{
    class Game : public IGame
    {
    private:
        GameSettings settings;
        Services services;
        std::unique_ptr<IGameMode> gameMode;

        Board board;
        Snake snake;

        std::vector<Position> apples;
        std::vector<IGameObserver*> observers;

        GameState state = GameState::Ready;
        int score = 0;

        void notifyAppleEaten();
        void notifyScoreChanged();
        void notifyGameOver();

        Position getNextHeadPosition() const;
        bool isAppleAt(Position position) const;
        void spawnApples();
        void finishGame();

    public:
        Game(
            const GameSettings& settings,
            const Services& services,
            std::unique_ptr<IGameMode> gameMode
        );

        void changeDirection(Direction direction) override;
        void tick() override;
        void start() override;
        void pause() override;
        void resume() override;

        GameState getState() const override;
        int getScore() const override;

        const std::vector<Position>& getSnakeBody() const override;
        const std::vector<Position>& getApples() const override;

        void addObserver(IGameObserver* observer) override;
        void removeObserver(IGameObserver* observer) override;
    };
}