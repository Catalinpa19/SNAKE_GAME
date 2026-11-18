#pragma once

#include "widgets/ArtImage.h"

#include <QColor>
#include <QString>

#include <array>
#include <span>

// LOGIC: these enums mirror the ones SnakeLogic will have in api/Enums.h (SnakeColor, MapColor, Achievement).
// Replace them with the logic ones. The UI only decides how each value looks: its name, its RGB and its art.
// The order of the values is the order of the tables in Catalog.cpp.
enum class SnakeColor
{
    Blue,
    LightBlue,
    Purple,
    Pink,
    Red,
    Orange,
    Yellow,
    Green,
    Black,
    White
};

enum class MapColor
{
    Sand,
    Clay,
    Copper,
    Burgundy,
    Mauve,
    Night,
    Slate,
    Ash,
    Charcoal,
    Bone
};

enum class Achievement
{
    Eat1,
    Eat3,
    Eat5,
    Eat10,
    Eat25,
    Eat50,
    Eat100,
    Die1,
    Die5,
    Die25,
    Win1,
    Win5,
    Win10
};

// The screens handle both color lists the same way, so they pass the kind and the enum value as an int.
enum class ColorKind
{
    Snake,
    Map
};

inline constexpr std::array kColorKinds{ColorKind::Snake, ColorKind::Map};

// LOGIC: the source, the price and the rewards below are decided by SnakeLogic (ISkinService,
// IAchievementService). They are sample data until the services exist.
enum class ColorSource
{
    Default,
    Achievement,
    Shop
};

struct ColorDef
{
    const char* name;
    QRgb base;  // the snake color, or the light tile of the map
    QRgb shade; // the dark tile of the map, unused for the snake
    ColorSource source;
    int price; // only used when the source is Shop
};

struct AchievementDef
{
    const char* title;
    const char* description;
    const char* icon;
    ColorKind rewardKind; // unused when the reward is drumsticks
    int rewardColor;      // enum value, or -1 when the reward is drumsticks
    int rewardDrumsticks;
};

struct ItemInfo
{
    QString label;
    ArtSpec art;
    ColorSource source = ColorSource::Default;
    int price = 0;
};

namespace Catalog
{
inline constexpr std::array kFruitCounts{1, 3, 5, 10};
inline constexpr std::array kSpeeds{"Slow", "Normal", "Fast"};
inline constexpr std::array kBoardSizes{"Small", "Medium", "Large"};

inline constexpr const char* kDefaultMode = "Classic";
inline constexpr const char* kDefaultSpeed = "Normal";
inline constexpr const char* kDefaultBoardSize = "Medium";

// The enum value of a color or of an achievement is its index in the list.
std::span<const ColorDef> colors(ColorKind kind);
std::span<const AchievementDef> achievements();

// Index in the list, or -1 when the name is unknown.
int boardSizeIndex(const QString& name);
int speedIndex(const QString& name);
int fruitCountIndex(int count);

QString text(const char* source);

ItemInfo colorInfo(ColorKind kind, int color);
ItemInfo rewardInfo(const AchievementDef& achievement);
} // namespace Catalog
