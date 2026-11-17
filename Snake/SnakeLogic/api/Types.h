
#pragma once

#include <string>
#include <memory>

#include "Enums.h"

namespace SnakeLogic
{

    class IProgressService;
    class ISkinService;
    class IAchievementService;


    
    struct Position
    {
        int row = 0;
        int column = 0;
    };


    struct GameSettings
    {
        GameModeId mode = GameModeId::Classic;
        BoardSize board = BoardSize::Medium;
        GameSpeed speed = GameSpeed::Normal;
        int appleCount = 1;
    };


   
    struct SkinInfo
    {
        SkinId id;
        std::string name;
        int price;
        bool unlocked;
    };


    
    struct AchievementInfo
    {
        AchievementId id;
        std::string name;
        std::string description;
        bool unlocked;
    };


    struct Services
    {
        std::shared_ptr<IProgressService> progress;
        std::shared_ptr<ISkinService> skins;
        std::shared_ptr<IAchievementService> achievements;
    };
}
