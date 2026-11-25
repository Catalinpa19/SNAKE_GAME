#include "GameModeRegistry.h"
#include "ClassicMode.h"

#include <utility>

namespace SnakeLogic
{
    GameModeRegistry::GameModeRegistry()
    {
        creators.emplace(
            GameModeId::Classic,
            []()
            {
                return std::make_unique<ClassicMode>();
            }
        );
    }

    GameModeRegistry& GameModeRegistry::instance()
    {
        static GameModeRegistry registry;
        return registry;
    }

    bool GameModeRegistry::registerMode(
        GameModeId id,
        Creator creator
    )
    {
        if (!creator)
        {
            return false;
        }

        return creators.emplace(id, std::move(creator)).second;
    }

    std::unique_ptr<IGameMode> GameModeRegistry::create(
        GameModeId id
    ) const
    {
        const auto it = creators.find(id);

        if (it == creators.end())
        {
            return nullptr;
        }

        return it->second();
    }
}