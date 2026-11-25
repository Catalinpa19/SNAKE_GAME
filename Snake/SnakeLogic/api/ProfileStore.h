#pragma once

#include "Enums.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace SnakeLogic
{
    class ProfileStore
    {
    private:
        std::string filePath;

        int applesBalance = 0;
        int applesLifetime = 0;
        int gamesPlayed = 0;
        SkinId equippedSkin = SkinId::Default;

        std::vector<SkinId> unlockedSkins{ SkinId::Default };
        std::vector<AchievementId> unlockedAchievements;
        std::unordered_map<int, int> highScores;

        void load();
        void save() const;

        static int getHighScoreKey(GameModeId mode, BoardSize board);

    public:
        explicit ProfileStore(const std::string& filePath);

        int getApplesBalance() const;
        int getApplesLifetime() const;
        int getGamesPlayed() const;

        int getHighScore(GameModeId mode, BoardSize board) const;

        void addApple();
        bool trySpendApples(int amount);

        void incrementGamesPlayed();
        void updateHighScore(GameModeId mode, BoardSize board, int score);

        std::vector<SkinId> getUnlockedSkins() const;
        SkinId getEquippedSkin() const;
        bool isSkinUnlocked(SkinId skin) const;
        bool unlockSkin(SkinId skin);
        bool equipSkin(SkinId skin);

        std::vector<AchievementId> getUnlockedAchievements() const;
        bool isAchievementUnlocked(AchievementId achievement) const;
        bool unlockAchievement(AchievementId achievement);
    };
}