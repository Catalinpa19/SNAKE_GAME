#pragma once

#include "IGameMode.h"

#include <functional>
#include <map>
#include <memory>

namespace SnakeLogic
{
    class GameModeRegistry
    {
    public:
        using Creator = std::function<std::unique_ptr<IGameMode>()>;

        static GameModeRegistry& instance();

        bool registerMode(GameModeId id, Creator creator);
        std::unique_ptr<IGameMode> create(GameModeId id) const;

    private:
        GameModeRegistry();

        std::map<GameModeId, Creator> creators;
    };
}