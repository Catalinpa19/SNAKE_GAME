#include "app/Catalog.h"

#include <QSet>

#include <gtest/gtest.h>

namespace
{
int colorCount(ColorKind kind)
{
    return static_cast<int>(Catalog::colors(kind).size());
}
} // namespace

TEST(Catalog, EachColorEnumHasOneDefinition)
{
    EXPECT_EQ(colorCount(ColorKind::Snake), static_cast<int>(SnakeColor::White) + 1);
    EXPECT_EQ(colorCount(ColorKind::Map), static_cast<int>(MapColor::Bone) + 1);
    EXPECT_EQ(static_cast<int>(Catalog::achievements().size()), static_cast<int>(Achievement::Win10) + 1);
}

TEST(Catalog, FirstColorOfEachKindIsFree)
{
    for (const ColorKind kind : kColorKinds)
        EXPECT_EQ(Catalog::colors(kind)[0].source, ColorSource::Default);
}

TEST(Catalog, EveryColorHasPresentationData)
{
    for (const ColorKind kind : kColorKinds)
    {
        for (int i = 0; i < colorCount(kind); ++i)
        {
            SCOPED_TRACE(i);
            const ColorDef& color = Catalog::colors(kind)[i];
            const ItemInfo info = Catalog::colorInfo(kind, i);

            EXPECT_FALSE(info.label.isEmpty());
            EXPECT_FALSE(info.art.name.isEmpty());
            EXPECT_EQ(info.source, color.source);
            EXPECT_EQ(info.price, color.price);
            EXPECT_EQ(color.source == ColorSource::Shop, color.price > 0);
        }
    }
}

TEST(Catalog, EveryAchievementHasAColorOrDrumstickReward)
{
    for (const AchievementDef& achievement : Catalog::achievements())
    {
        SCOPED_TRACE(achievement.title);
        EXPECT_NE(QString(achievement.title), QString());
        EXPECT_NE(QString(achievement.description), QString());

        if (achievement.rewardColor < 0)
        {
            EXPECT_GT(achievement.rewardDrumsticks, 0);
            continue;
        }
        ASSERT_LT(achievement.rewardColor, colorCount(achievement.rewardKind));
        EXPECT_EQ(Catalog::colors(achievement.rewardKind)[achievement.rewardColor].source, ColorSource::Achievement);
    }
}

TEST(Catalog, EveryAchievementColorIsRewardedByExactlyOneAchievement)
{
    QSet<QPair<int, int>> rewarded;
    for (const AchievementDef& achievement : Catalog::achievements())
    {
        if (achievement.rewardColor < 0)
            continue;
        const QPair<int, int> reward(static_cast<int>(achievement.rewardKind), achievement.rewardColor);
        EXPECT_FALSE(rewarded.contains(reward)) << achievement.title;
        rewarded.insert(reward);
    }

    for (const ColorKind kind : kColorKinds)
    {
        for (int i = 0; i < colorCount(kind); ++i)
        {
            const bool isReward = Catalog::colors(kind)[i].source == ColorSource::Achievement;
            EXPECT_EQ(rewarded.contains({static_cast<int>(kind), i}), isReward);
        }
    }
}

TEST(Catalog, RewardInfoDescribesColorsAndDrumsticks)
{
    for (const AchievementDef& achievement : Catalog::achievements())
    {
        const ItemInfo info = Catalog::rewardInfo(achievement);
        EXPECT_FALSE(info.label.isEmpty());
        if (achievement.rewardColor < 0)
            EXPECT_EQ(info.art.name, "drumstick");
    }
}

TEST(Catalog, OptionValuesRoundTripAndUnknownValuesAreRejected)
{
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_EQ(Catalog::speedIndex(QLatin1StringView(Catalog::kSpeeds[i])), i);
        EXPECT_EQ(Catalog::boardSizeIndex(QLatin1StringView(Catalog::kBoardSizes[i])), i);
    }
    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(Catalog::fruitCountIndex(Catalog::kFruitCounts[i]), i);

    EXPECT_EQ(Catalog::speedIndex("unknown"), -1);
    EXPECT_EQ(Catalog::boardSizeIndex("unknown"), -1);
    EXPECT_EQ(Catalog::fruitCountIndex(2), -1);
}
