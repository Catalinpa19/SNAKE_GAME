#pragma once

#include "ISkinService.h"
#include "IProgressService.h"
#include "ProfileStore.h"

#include <memory>
#include <vector>

namespace SnakeLogic
{
    class SkinService : public ISkinService
    {
    private:
        std::shared_ptr<ProfileStore> profile;
        std::shared_ptr<IProgressService> progress;

    public:
        SkinService(
            std::shared_ptr<ProfileStore> profile,
            std::shared_ptr<IProgressService> progress
        );

        std::vector<SkinInfo> getAvailableSkins() const override;
        SkinId getEquippedSkin() const override;

        bool unlockSkin(SkinId skin) override;
        bool equipSkin(SkinId skin) override;
    };
}