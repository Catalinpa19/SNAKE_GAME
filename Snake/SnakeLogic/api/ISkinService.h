
#pragma once

#include "Enums.h"
#include "Types.h"

#include <vector>

namespace SnakeLogic
{
    class ISkinService
    {
    public:
        virtual ~ISkinService() = default;

        virtual std::vector<SkinInfo> getAvailableSkins() const = 0;

        virtual SkinId getEquippedSkin() const = 0;

        virtual bool unlockSkin(SkinId skin) = 0;

        virtual bool equipSkin(SkinId skin) = 0;
    };
}
