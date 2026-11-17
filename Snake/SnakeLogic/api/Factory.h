
#pragma once

#include "Types.h"
#include "IGame.h"
#include "IProgressService.h"
#include "ISkinService.h"
#include "IAchievementService.h"

#include <memory>
#include <string>

namespace SnakeLogic
{
    class Factory
    {
    public:
        static Services createServices(
            const std::string& profilePath
        );

        static std::unique_ptr<IGame> createGame(
            const GameSettings& settings,
            const Services& services
        );
    };
}
