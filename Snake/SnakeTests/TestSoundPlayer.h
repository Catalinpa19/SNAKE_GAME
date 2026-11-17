#pragma once
#include "app/SoundPlayer.h"
#include <gmock/gmock.h>

// Link-time audio substitute. Widgets still use the production SoundPlayer API.
// A scoped observer records requests without loading devices or playing WAV files.
class SoundObserver {
public:
    SoundObserver();
    ~SoundObserver();
    SoundObserver(const SoundObserver&) = delete;
    SoundObserver& operator=(const SoundObserver&) = delete;
    ::testing::StrictMock<::testing::MockFunction<void(Sound)>> requests;
};
