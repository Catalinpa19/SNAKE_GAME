#include "TestSoundPlayer.h"
#include <cassert>
namespace { SoundObserver* observer = nullptr; }
SoundObserver::SoundObserver() { assert(observer == nullptr); observer = this; }
SoundObserver::~SoundObserver() { observer = nullptr; }
SoundPlayer::SoundPlayer(QObject* parent) : QObject(parent) {}
void SoundPlayer::setEnabled(bool enabled) { m_enabled = enabled; }
void SoundPlayer::play(Sound sound) {
    if (m_enabled && observer) observer->requests.Call(sound);
}
