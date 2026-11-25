#include "ProfileStore.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <system_error>

namespace SnakeLogic
{
    namespace
    {
        int readInt(
            const std::string& json,
            const std::string& key,
            int defaultValue
        )
        {
            const std::regex pattern(
                "\"" + key + "\"\\s*:\\s*(-?[0-9]+)"
            );

            std::smatch match;

            if (!std::regex_search(json, match, pattern))
            {
                return defaultValue;
            }

            try
            {
                return std::stoi(match[1].str());
            }
            catch (...)
            {
                return defaultValue;
            }
        }

        std::string readArray(
            const std::string& json,
            const std::string& key
        )
        {
            const std::regex pattern(
                "\"" + key + "\"\\s*:\\s*\\[([^\\]]*)\\]"
            );

            std::smatch match;

            if (!std::regex_search(json, match, pattern))
            {
                return {};
            }

            return match[1].str();
        }

        std::vector<int> readIntArray(const std::string& content)
        {
            std::vector<int> values;
            const std::regex pattern("-?[0-9]+");

            for (
                auto it = std::sregex_iterator(
                    content.begin(),
                    content.end(),
                    pattern
                );
                it != std::sregex_iterator();
                ++it
                )
            {
                try
                {
                    values.push_back(std::stoi(it->str()));
                }
                catch (...)
                {
                }
            }

            return values;
        }

        std::string readObject(
            const std::string& json,
            const std::string& key
        )
        {
            const std::regex pattern(
                "\"" + key + "\"\\s*:\\s*\\{([^}]*)\\}"
            );

            std::smatch match;

            if (!std::regex_search(json, match, pattern))
            {
                return {};
            }

            return match[1].str();
        }

        std::unordered_map<int, int> readIntMap(const std::string& content)
        {
            std::unordered_map<int, int> values;

            const std::regex pattern(
                "\"([0-9]+)\"\\s*:\\s*(-?[0-9]+)"
            );

            for (
                auto it = std::sregex_iterator(
                    content.begin(),
                    content.end(),
                    pattern
                );
                it != std::sregex_iterator();
                ++it
                )
            {
                try
                {
                    const int key = std::stoi((*it)[1].str());
                    const int value = std::stoi((*it)[2].str());

                    values[key] = value;
                }
                catch (...)
                {
                }
            }

            return values;
        }

        bool isValidSkin(int value)
        {
            return value >= static_cast<int>(SkinId::Default) &&
                value <= static_cast<int>(SkinId::Neon);
        }

        bool isValidAchievement(int value)
        {
            return value >= static_cast<int>(AchievementId::FirstApple) &&
                value <= static_cast<int>(AchievementId::GamesPlayed10);
        }
    }

    ProfileStore::ProfileStore(const std::string& path)
        : filePath(path)
    {
        load();
    }

    void ProfileStore::load()
    {
        if (filePath.empty())
        {
            return;
        }

        std::ifstream input(filePath);

        if (!input)
        {
            save();
            return;
        }

        const std::string json(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()
        );

        applesBalance = std::max(0, readInt(json, "applesBalance", 0));
        applesLifetime = std::max(0, readInt(json, "applesLifetime", 0));
        gamesPlayed = std::max(0, readInt(json, "gamesPlayed", 0));

        unlockedSkins.clear();

        for (const int value : readIntArray(readArray(json, "unlockedSkins")))
        {
            if (!isValidSkin(value))
            {
                continue;
            }

            const SkinId skin = static_cast<SkinId>(value);

            if (std::find(unlockedSkins.begin(), unlockedSkins.end(), skin)
                == unlockedSkins.end())
            {
                unlockedSkins.push_back(skin);
            }
        }

        if (std::find(
            unlockedSkins.begin(),
            unlockedSkins.end(),
            SkinId::Default) == unlockedSkins.end())
        {
            unlockedSkins.push_back(SkinId::Default);
        }

        const int equippedValue = readInt(json, "equippedSkin", 0);

        if (isValidSkin(equippedValue) &&
            isSkinUnlocked(static_cast<SkinId>(equippedValue)))
        {
            equippedSkin = static_cast<SkinId>(equippedValue);
        }
        else
        {
            equippedSkin = SkinId::Default;
        }

        unlockedAchievements.clear();

        for (const int value : readIntArray(
            readArray(json, "unlockedAchievements")))
        {
            if (!isValidAchievement(value))
            {
                continue;
            }

            const AchievementId achievement =
                static_cast<AchievementId>(value);

            if (std::find(
                unlockedAchievements.begin(),
                unlockedAchievements.end(),
                achievement) == unlockedAchievements.end())
            {
                unlockedAchievements.push_back(achievement);
            }
        }

        highScores = readIntMap(readObject(json, "highScores"));

        for (auto& entry : highScores)
        {
            entry.second = std::max(0, entry.second);
        }
    }

    void ProfileStore::save() const
    {
        if (filePath.empty())
        {
            return;
        }

        const std::filesystem::path path(filePath);

        if (!path.parent_path().empty())
        {
            std::error_code directoryError;

            std::filesystem::create_directories(
                path.parent_path(),
                directoryError
            );

            if (directoryError)
            {
                return;
            }
        }

        const std::string temporaryPath = filePath + ".tmp";

        std::ofstream output(
            temporaryPath,
            std::ios::out | std::ios::trunc
        );

        if (!output)
        {
            return;
        }

        output << "{\n";
        output << "  \"applesBalance\": " << applesBalance << ",\n";
        output << "  \"applesLifetime\": " << applesLifetime << ",\n";
        output << "  \"gamesPlayed\": " << gamesPlayed << ",\n";
        output << "  \"equippedSkin\": "
            << static_cast<int>(equippedSkin) << ",\n";

        output << "  \"unlockedSkins\": [";

        for (std::size_t i = 0; i < unlockedSkins.size(); ++i)
        {
            if (i > 0)
            {
                output << ", ";
            }

            output << static_cast<int>(unlockedSkins[i]);
        }

        output << "],\n";
        output << "  \"unlockedAchievements\": [";

        for (std::size_t i = 0; i < unlockedAchievements.size(); ++i)
        {
            if (i > 0)
            {
                output << ", ";
            }

            output << static_cast<int>(unlockedAchievements[i]);
        }

        output << "],\n";
        output << "  \"highScores\": {";

        for (int key = 0; key < 3; ++key)
        {
            if (key > 0)
            {
                output << ", ";
            }

            const auto it = highScores.find(key);
            const int value = it == highScores.end() ? 0 : it->second;

            output << "\"" << key << "\": " << value;
        }

        output << "}\n";
        output << "}\n";

        output.close();

        if (!output)
        {
            std::error_code removeError;
            std::filesystem::remove(temporaryPath, removeError);
            return;
        }

        std::error_code renameError;

        std::filesystem::rename(
            temporaryPath,
            filePath,
            renameError
        );

        if (renameError)
        {
            std::error_code removeError;
            std::filesystem::remove(filePath, removeError);

            renameError.clear();

            std::filesystem::rename(
                temporaryPath,
                filePath,
                renameError
            );
        }

        if (renameError)
        {
            std::error_code removeError;
            std::filesystem::remove(temporaryPath, removeError);
        }
    }

    int ProfileStore::getHighScoreKey(
        GameModeId mode,
        BoardSize board
    )
    {
        return static_cast<int>(mode) * 3 +
            static_cast<int>(board);
    }

    int ProfileStore::getApplesBalance() const
    {
        return applesBalance;
    }

    int ProfileStore::getApplesLifetime() const
    {
        return applesLifetime;
    }

    int ProfileStore::getGamesPlayed() const
    {
        return gamesPlayed;
    }

    int ProfileStore::getHighScore(
        GameModeId mode,
        BoardSize board
    ) const
    {
        const int key = getHighScoreKey(mode, board);
        const auto it = highScores.find(key);

        return it == highScores.end() ? 0 : it->second;
    }

    void ProfileStore::addApple()
    {
        ++applesBalance;
        ++applesLifetime;
        save();
    }

    bool ProfileStore::trySpendApples(int amount)
    {
        if (amount < 0 || applesBalance < amount)
        {
            return false;
        }

        applesBalance -= amount;
        save();

        return true;
    }

    void ProfileStore::incrementGamesPlayed()
    {
        ++gamesPlayed;
        save();
    }

    void ProfileStore::updateHighScore(
        GameModeId mode,
        BoardSize board,
        int score
    )
    {
        if (score < 0)
        {
            return;
        }

        const int key = getHighScoreKey(mode, board);
        int& currentHighScore = highScores[key];

        if (score > currentHighScore)
        {
            currentHighScore = score;
            save();
        }
    }

    std::vector<SkinId> ProfileStore::getUnlockedSkins() const
    {
        return unlockedSkins;
    }

    SkinId ProfileStore::getEquippedSkin() const
    {
        return equippedSkin;
    }

    bool ProfileStore::isSkinUnlocked(SkinId skin) const
    {
        return std::find(
            unlockedSkins.begin(),
            unlockedSkins.end(),
            skin
        ) != unlockedSkins.end();
    }

    bool ProfileStore::unlockSkin(SkinId skin)
    {
        if (isSkinUnlocked(skin))
        {
            return false;
        }

        unlockedSkins.push_back(skin);
        save();

        return true;
    }

    bool ProfileStore::equipSkin(SkinId skin)
    {
        if (!isSkinUnlocked(skin))
        {
            return false;
        }

        equippedSkin = skin;
        save();

        return true;
    }

    std::vector<AchievementId>
        ProfileStore::getUnlockedAchievements() const
    {
        return unlockedAchievements;
    }

    bool ProfileStore::isAchievementUnlocked(
        AchievementId achievement
    ) const
    {
        return std::find(
            unlockedAchievements.begin(),
            unlockedAchievements.end(),
            achievement
        ) != unlockedAchievements.end();
    }

    bool ProfileStore::unlockAchievement(AchievementId achievement)
    {
        if (isAchievementUnlocked(achievement))
        {
            return false;
        }

        unlockedAchievements.push_back(achievement);
        save();

        return true;
    }
}