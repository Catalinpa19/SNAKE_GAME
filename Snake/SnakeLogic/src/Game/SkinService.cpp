#include "SkinService.h"

#include <stdexcept>
#include <utility>

namespace SnakeLogic
{
    namespace
    {
        constexpr int NeonSkinPrice = 50;
    }

    SkinService::SkinService(
        std::shared_ptr<ProfileStore> profileStore,
        std::shared_ptr<IProgressService> progressService
    )
        : profile(std::move(profileStore)),
        progress(std::move(progressService))
    {
        if (!profile || !progress)
        {
            throw std::invalid_argument("Skin service dependencies cannot be null");
        }
    }

    std::vector<SkinInfo> SkinService::getAvailableSkins() const
    {
        return {
            {
                SkinId::Default,
                "Default",
                0,
                profile->isSkinUnlocked(SkinId::Default)
            },
            {
                SkinId::Neon,
                "Neon",
                NeonSkinPrice,
                profile->isSkinUnlocked(SkinId::Neon)
            }
        };
    }

    SkinId SkinService::getEquippedSkin() const
    {
        return profile->getEquippedSkin();
    }

    bool SkinService::unlockSkin(SkinId skin)
    {
        if (profile->isSkinUnlocked(skin))
        {
            return true;
        }

        if (skin == SkinId::Default)
        {
            return profile->unlockSkin(skin);
        }

        if (skin != SkinId::Neon)
        {
            return false;
        }

        if (!progress->trySpendApples(NeonSkinPrice))
        {
            return false;
        }

        return profile->unlockSkin(skin);
    }

    bool SkinService::equipSkin(SkinId skin)
    {
        return profile->equipSkin(skin);
    }
}