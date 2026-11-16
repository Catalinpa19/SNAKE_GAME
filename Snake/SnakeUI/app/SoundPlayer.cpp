#include "app/SoundPlayer.h"

#include <QDebug>
#include <QtMultimedia/QSoundEffect>
#include <QUrl>

namespace
{
constexpr std::array kFiles{"up", "down", "left", "right", "eat", "hurt", "win"};
} // namespace

SoundPlayer::SoundPlayer(QObject* parent)
    : QObject(parent)
{
    for (size_t i = 0; i < m_effects.size(); ++i)
    {
        auto* effect = new QSoundEffect(this);
        effect->setSource(QUrl(QStringLiteral("qrc:/sounds/%1.wav").arg(QLatin1StringView(kFiles[i]))));
        connect(effect, &QSoundEffect::statusChanged, this, [effect] {
            if (effect->status() == QSoundEffect::Error)
                qWarning() << "Could not load sound" << effect->source();
        });
        m_effects[i] = effect;
    }
}

void SoundPlayer::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void SoundPlayer::play(Sound sound)
{
    if (m_enabled)
        m_effects[static_cast<size_t>(sound)]->play();
}
