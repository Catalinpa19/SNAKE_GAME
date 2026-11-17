#include <gtest/gtest.h>
#include "app/ProfileStore.h"
#include "app/NumberFormat.h"
#include <QTemporaryDir>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct FormatCase { qint64 value; const char* expected; };
class NumberFormatTest : public ::testing::TestWithParam<FormatCase> {};
TEST_P(NumberFormatTest, FormatsAndTruncatesAtUnitBoundaries) { EXPECT_EQ(formatCount(GetParam().value), GetParam().expected); }
INSTANTIATE_TEST_SUITE_P(Boundaries, NumberFormatTest, ::testing::Values(
    FormatCase{-1,"-1"}, FormatCase{0,"0"}, FormatCase{1,"1"}, FormatCase{999,"999"}, FormatCase{1000,"1k"},
    FormatCase{1099,"1k"}, FormatCase{1100,"1.1k"}, FormatCase{1500,"1.5k"}, FormatCase{999999,"999.9k"},
    FormatCase{1000000,"1m"}, FormatCase{1999999,"1.9m"}, FormatCase{1000000000,"1b"}, FormatCase{2147483647,"2.1b"}));

class ProfileTest : public ::testing::Test {
protected:
    QTemporaryDir dir;
    QString path() { return dir.filePath("nested/profile.json"); }
    void write(const QByteArray& bytes) {
        QDir().mkpath(QFileInfo(path()).absolutePath()); QFile file(path()); ASSERT_TRUE(file.open(QIODevice::WriteOnly)); ASSERT_EQ(file.write(bytes), bytes.size());
    }
};
TEST_F(ProfileTest, MissingFileCreatesDefaultsAndDirectories) {
    ASSERT_TRUE(dir.isValid()); ProfileStore store(path()); EXPECT_TRUE(QFile::exists(path())); EXPECT_EQ(store.data().applesBalance, 0);
    EXPECT_EQ(store.data().settings.board, "Medium"); EXPECT_EQ(store.data().settings.speed, "Normal"); EXPECT_EQ(store.data().settings.appleCount, 1);
    for (auto kind : kColorKinds) EXPECT_TRUE(store.data().unlocked(kind).contains(store.data().equipped(kind)));
}
TEST_F(ProfileTest, InvalidJsonRecoversDefaults) { write("{broken"); ProfileStore store(path()); EXPECT_EQ(store.data().applesBalance, 0); EXPECT_TRUE(store.save()); }
TEST_F(ProfileTest, NonObjectJsonRecoversDefaults) { write("[]"); ProfileStore store(path()); EXPECT_EQ(store.data().settings.board, "Medium"); }
TEST_F(ProfileTest, InvalidSettingsAndNegativeBalancesAreSanitized) {
    write(R"({"applesBalance":-1,"applesLifetime":-5,"settings":{"mode":"Invalid","board":"Bad","speed":"Bad","appleCount":2}})");
    ProfileStore store(path()); EXPECT_EQ(store.data().applesBalance,0); EXPECT_EQ(store.data().applesLifetime,0);
    EXPECT_EQ(store.data().settings.mode,"Classic"); EXPECT_EQ(store.data().settings.board,"Medium"); EXPECT_EQ(store.data().settings.speed,"Normal"); EXPECT_EQ(store.data().settings.appleCount,1);
}
TEST_F(ProfileTest, SaveReloadPreservesSettingsScoresAndBalances) {
    { ProfileStore store(path()); auto& d=store.data(); d.applesBalance=123; d.applesLifetime=456; d.settings.board="Large"; d.settings.speed="Fast";
      d.settings.appleCount=10; d.settings.audioEnabled=false; d.highScores["Classic/Large"]=42; ASSERT_TRUE(store.save()); }
    ProfileStore store(path()); EXPECT_EQ(store.data().applesBalance,123); EXPECT_EQ(store.data().applesLifetime,456); EXPECT_EQ(store.data().bestScore(),42);
    EXPECT_EQ(store.data().settings.speed,"Fast"); EXPECT_EQ(store.data().settings.appleCount,10); EXPECT_FALSE(store.data().settings.audioEnabled);
}
TEST_F(ProfileTest, HighScoresAreSpecificToBoardAndDefaultToZero) {
    ProfileStore store(path()); store.data().highScores["Classic/Small"]=10; EXPECT_EQ(store.data().bestScore(),0);
    store.data().settings.board="Small"; EXPECT_EQ(store.data().bestScore(),10);
}
TEST_F(ProfileTest, UnknownAchievementAndColorDoNotMutateBalance) {
    ProfileStore store(path()); store.data().applesBalance=1000; EXPECT_FALSE(store.unlockAchievement("unknown"));
    for (auto kind : kColorKinds) EXPECT_FALSE(store.purchaseColor(kind,"unknown")); EXPECT_EQ(store.data().applesBalance,1000);
}
TEST_F(ProfileTest, EveryShopColorChecksFundsChargesOnceAndPersists) {
    for (auto kind : kColorKinds) for (const auto& color : Catalog::colors(kind)) {
        if (color.source != ColorSource::Shop) continue;
        SCOPED_TRACE(color.id); ProfileStore store(path()); const QString id=QLatin1StringView(color.id);
        store.data().applesBalance=color.price-1; EXPECT_FALSE(store.purchaseColor(kind,id)); EXPECT_FALSE(store.data().unlocked(kind).contains(id));
        store.data().applesBalance=color.price; EXPECT_TRUE(store.purchaseColor(kind,id)); EXPECT_EQ(store.data().applesBalance,0);
        EXPECT_FALSE(store.purchaseColor(kind,id)); store.data().equipped(kind)=id; ASSERT_TRUE(store.save());
        ProfileStore loaded(path()); EXPECT_TRUE(loaded.data().unlocked(kind).contains(id)); EXPECT_EQ(loaded.data().equipped(kind),id);
    }
}
TEST_F(ProfileTest, EveryAchievementGrantsRewardOnlyOnceIncludingAfterReload) {
    ProfileStore store(path()); int balance=0;
    for (const auto& a : Catalog::achievements()) {
        SCOPED_TRACE(a.id); const QString id=QLatin1StringView(a.id); EXPECT_TRUE(store.unlockAchievement(id)); EXPECT_FALSE(store.unlockAchievement(id));
        if (a.rewardColorId) EXPECT_TRUE(store.data().unlocked(a.rewardKind).contains(QLatin1StringView(a.rewardColorId)));
        else balance+=a.rewardDrumsticks;
        EXPECT_EQ(store.data().applesBalance,balance);
    }
    ASSERT_TRUE(store.save()); ProfileStore loaded(path()); EXPECT_EQ(loaded.data().applesBalance,balance);
    for (const auto& a : Catalog::achievements()) EXPECT_FALSE(loaded.unlockAchievement(QLatin1StringView(a.id)));
}
TEST(Catalog, DefinitionsAreConsistent) { EXPECT_TRUE(Catalog::isConsistent()); }
TEST(Catalog, EveryColorRoundTripsAndHasPresentationData) {
    for (auto kind : kColorKinds) { auto colors=Catalog::colors(kind);
        for (int i=0; i<static_cast<int>(colors.size()); ++i) {
            const QString id=QLatin1StringView(colors[i].id); EXPECT_EQ(Catalog::colorIndex(kind,id),i);
            const auto info=Catalog::colorInfo(kind,id); EXPECT_FALSE(info.label.isEmpty()); EXPECT_FALSE(info.art.name.isEmpty());
            EXPECT_EQ(info.price,colors[i].price); EXPECT_EQ(info.source,colors[i].source);
        }
        EXPECT_EQ(Catalog::colorIndex(kind,"unknown"),-1); EXPECT_GE(Catalog::colorIndex(kind,Catalog::defaultColorId(kind)),0);
    }
}
TEST(Catalog, AllOptionValuesRoundTripAndUnknownValuesAreRejected) {
    for (int i=0;i<3;++i) { EXPECT_EQ(Catalog::speedIndex(QLatin1StringView(Catalog::kSpeeds[i])),i); EXPECT_EQ(Catalog::boardSizeIndex(QLatin1StringView(Catalog::kBoardSizes[i])),i); }
    for (int i=0;i<4;++i) EXPECT_EQ(Catalog::fruitCountIndex(Catalog::kFruitCounts[i]),i);
    EXPECT_EQ(Catalog::speedIndex("unknown"),-1); EXPECT_EQ(Catalog::boardSizeIndex("unknown"),-1); EXPECT_EQ(Catalog::fruitCountIndex(2),-1); EXPECT_EQ(Catalog::achievementIndex("unknown"),-1);
}
