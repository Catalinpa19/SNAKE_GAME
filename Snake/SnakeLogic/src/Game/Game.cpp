#include "Game.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace SnakeLogic
{
    namespace
    {
        int getBoardDimension(BoardSize size)
        {
            switch (size)
            {
            case BoardSize::Small:
                return 15;
            case BoardSize::Medium:
                return 25;
            case BoardSize::Large:
                return 35;
            }

            return 25;
        }

        std::vector<Position> createInitialSnake(int width, int height)
        {
            const int row = height / 2;
            const int column = width / 2;

            return {
                { row, column },
                { row, column - 1 },
                { row, column - 2 }
            };
        }

        Position getNextPosition(Position head, Direction direction)
        {
            switch (direction)
            {
            case Direction::Up:
                --head.row;
                break;
            case Direction::Down:
                ++head.row;
                break;
            case Direction::Left:
                --head.column;
                break;
            case Direction::Right:
                ++head.column;
                break;
            }

            return head;
        }
    }

    Game::Game(
        const GameSettings& gameSettings,
        const Services& gameServices,
        std::unique_ptr<IGameMode> mode
    )
        : settings(gameSettings),
        services(gameServices),
        gameMode(std::move(mode)),
        board(
            getBoardDimension(gameSettings.board),
            getBoardDimension(gameSettings.board)
        ),
        snake(
            createInitialSnake(
                getBoardDimension(gameSettings.board),
                getBoardDimension(gameSettings.board)
            ),
            Direction::Right
        )
    {
        if (!gameMode)
        {
            throw std::invalid_argument("Game mode cannot be null");
        }

        gameMode->setupBoard(settings);

        if (services.progress)
        {
            addObserver(services.progress.get());
        }

        if (services.achievements)
        {
            addObserver(services.achievements.get());
        }

        spawnApples();
    }

    void Game::changeDirection(Direction direction)
    {
        if (state == GameState::GameOver)
        {
            return;
        }

        snake.changeDirection(direction);
    }

    void Game::tick()
    {
        if (state != GameState::Running)
        {
            return;
        }

        gameMode->onTick();

        const Position nextHead = getNextHeadPosition();

        if (gameMode->isCollision(
            nextHead,
            snake.getBody(),
            board.getWidth(),
            board.getHeight()))
        {
            finishGame();
            return;
        }

        const bool eatingApple = isAppleAt(nextHead);

        snake.move(eatingApple);

        if (eatingApple)
        {
            apples.erase(
                std::remove(apples.begin(), apples.end(), nextHead),
                apples.end()
            );

            ++score;
            gameMode->onAppleEaten();

            notifyAppleEaten();
            notifyScoreChanged();

            spawnApples();
        }
    }

    void Game::start()
    {
        if (state == GameState::Ready)
        {
            state = GameState::Running;
        }
    }

    void Game::pause()
    {
        if (state == GameState::Running)
        {
            state = GameState::Paused;
        }
    }

    void Game::resume()
    {
        if (state == GameState::Paused)
        {
            state = GameState::Running;
        }
    }

    GameState Game::getState() const
    {
        return state;
    }

    int Game::getScore() const
    {
        return score;
    }

    const std::vector<Position>& Game::getSnakeBody() const
    {
        return snake.getBody();
    }

    const std::vector<Position>& Game::getApples() const
    {
        return apples;
    }

    void Game::addObserver(IGameObserver* observer)
    {
        if (observer == nullptr)
        {
            return;
        }

        const auto it = std::find(
            observers.begin(),
            observers.end(),
            observer
        );

        if (it == observers.end())
        {
            observers.push_back(observer);
        }
    }

    void Game::removeObserver(IGameObserver* observer)
    {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    void Game::notifyAppleEaten()
    {
        for (IGameObserver* observer : observers)
        {
            observer->onAppleEaten();
        }
    }

    void Game::notifyScoreChanged()
    {
        for (IGameObserver* observer : observers)
        {
            observer->onScoreChanged(score);
        }
    }

    void Game::notifyGameOver()
    {
        for (IGameObserver* observer : observers)
        {
            observer->onGameOver(score);
        }
    }

    Position Game::getNextHeadPosition() const
    {
        return getNextPosition(snake.getHead(), snake.getDirection());
    }

    bool Game::isAppleAt(Position position) const
    {
        return std::find(apples.begin(), apples.end(), position) != apples.end();
    }

    void Game::spawnApples()
    {
        const int targetAppleCount = std::max(0, settings.appleCount);

        std::vector<Position> occupiedPositions = snake.getBody();

        for (const Position& apple : apples)
        {
            occupiedPositions.push_back(apple);
        }

        while (static_cast<int>(apples.size()) < targetAppleCount)
        {
            const Position apple = gameMode->spawnApple(
                occupiedPositions,
                board.getWidth(),
                board.getHeight()
            );

            if (!board.isInside(apple.row, apple.column))
            {
                break;
            }

            if (std::find(
                occupiedPositions.begin(),
                occupiedPositions.end(),
                apple) != occupiedPositions.end())
            {
                break;
            }

            apples.push_back(apple);
            occupiedPositions.push_back(apple);
        }
    }

    void Game::finishGame()
    {
        if (state == GameState::GameOver)
        {
            return;
        }

        state = GameState::GameOver;
        notifyGameOver();
    }
}