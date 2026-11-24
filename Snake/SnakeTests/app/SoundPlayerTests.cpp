#include "app/SoundPlayer.h"

#include <QFile>

#include <gtest/gtest.h>

TEST(SoundPlayer, EverySoundFileIsEmbeddedInTheResources)
{
    for (const char* name : {"up", "down", "left", "right", "eat", "hurt", "win"})
    {
        QFile file(QStringLiteral(":/sounds/%1.wav").arg(QLatin1StringView(name)));
        EXPECT_TRUE(file.open(QIODevice::ReadOnly)) << name;
        EXPECT_GT(file.size(), 0) << name;
    }
}

TEST(SoundPlayer, DisabledPlayerAcceptsEverySound)
{
    SoundPlayer player;
    player.setEnabled(false);

    for (const Sound sound : {Sound::Up, Sound::Down, Sound::Left, Sound::Right, Sound::Eat, Sound::Hurt, Sound::Win})
        player.play(sound);
}