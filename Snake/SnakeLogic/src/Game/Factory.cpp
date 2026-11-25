#include "Factory.h"

#include "Game.h"
#include "GameModeRegistry.h"
#include "ProfileStore.h"
#include "ProgressService.h"
#include "SkinService.h"
#include "AchievementService.h"

#include <memory>
#include <stdexcept>

namespace SnakeLogic
{
    Services Factory::createServices(const std::string& profilePath)
    {
        Services services;

        auto profile = std::make_shared<ProfileStore>(profilePath);

        services.progress = std::make_shared<ProgressService>(profile);

        services.skins = std::make_shared<SkinService>(
            profile,
            services.progress
        );

        services.achievements =
            std::make_shared<AchievementService>(profile);

        return services;
    }

    std::unique_ptr<IGame> Factory::createGame(
        const GameSettings& settings,
        const Services& services
    )
    {
        auto mode = GameModeRegistry::instance().create(settings.mode);

        if (!mode)
        {
            throw std::invalid_argument("Unsupported game mode");
        }

        return std::make_unique<Game>(
            settings,
            services,
            std::move(mode)
        );
    }
}