#pragma once

#include "app/Catalog.h"

#include <QList>
#include <QString>

#include <array>

struct Selections
{
    QString mode = QLatin1StringView(Catalog::kDefaultMode);
    QString board = QLatin1StringView(Catalog::kDefaultBoardSize);
    QString speed = QLatin1StringView(Catalog::kDefaultSpeed);
    int appleCount = 1;
};

// What the screens show. The UI only keeps what the player picked: nothing is saved, nothing is decided here.
// Colors and achievements are stored as the value of their enum.
struct UiState
{
    // LOGIC: the picks become the GameSettings (mode, board, appleCount, speed) given to createGame().
    // The colors only change how the UI draws the snake and the board.
    Selections settings;
    std::array<int, kColorKinds.size()> equippedColors{};

    // LOGIC: owned by SnakeLogic and read from its services. Placeholders until they exist:
    int applesBalance = 0;                                   // IProgressService::getApplesBalance()
    int bestScore = 0;                                       // IProgressService::getHighScore(mode, board)
    std::array<QList<int>, kColorKinds.size()> unlockedColors; // ISkinService::getAvailableSkins()
    QList<int> unlockedAchievements;                         // IAchievementService::getAchievements()

    UiState()
    {
        for (const ColorKind kind : kColorKinds)
        {
            const auto colors = Catalog::colors(kind);
            for (int i = 0; i < static_cast<int>(colors.size()); ++i)
            {
                if (colors[i].source == ColorSource::Default)
                    unlocked(kind).append(i);
            }
            equipped(kind) = unlocked(kind).first();
        }
    }

    int& equipped(ColorKind kind) { return equippedColors[static_cast<size_t>(kind)]; }
    int equipped(ColorKind kind) const { return equippedColors[static_cast<size_t>(kind)]; }
    QList<int>& unlocked(ColorKind kind) { return unlockedColors[static_cast<size_t>(kind)]; }
    const QList<int>& unlocked(ColorKind kind) const { return unlockedColors[static_cast<size_t>(kind)]; }
};
