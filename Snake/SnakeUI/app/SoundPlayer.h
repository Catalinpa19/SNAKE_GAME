#pragma once

#include <QObject>

#include <array>

class QSoundEffect;

enum class Sound
{
    Up,
    Down,
    Left,
    Right,
    Eat,
    Hurt,
    Win
};

// LOGIC: Eat, Hurt and Win are played when QtGameObserver reports onAppleEaten and onGameOver (win or
// loss). The four direction sounds stay in the key handling of GameScreen.
class SoundPlayer : public QObject
{
public:
    explicit SoundPlayer(QObject* parent = nullptr);

    void setEnabled(bool enabled);
    void play(Sound sound);

private:
    std::array<QSoundEffect*, 7> m_effects{};
    bool m_enabled = true;
};
