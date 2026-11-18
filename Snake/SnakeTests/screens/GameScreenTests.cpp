#include "screens/GameScreen.h"

#include "app/SoundPlayer.h"
#include "widgets/GuideWidget.h"

#include <QLabel>
#include <QTest>

#include <gtest/gtest.h>

namespace
{
GuideWidget* findGuide(GameScreen& screen)
{
    for (auto* widget : screen.findChildren<QWidget*>())
    {
        if (auto* guide = dynamic_cast<GuideWidget*>(widget))
            return guide;
    }
    return nullptr;
}

class DirectionKeyTest : public ::testing::TestWithParam<int>
{
};
} // namespace

TEST(GameScreen, EscapeRequestsPause)
{
    SoundPlayer sound;
    sound.setEnabled(false);
    GameScreen screen(sound);
    int requests = 0;
    QObject::connect(&screen, &GameScreen::pauseRequested, &screen, [&] { ++requests; });

    QTest::keyClick(&screen, Qt::Key_Escape);

    EXPECT_EQ(requests, 1);
}

TEST(GameScreen, ScoreIsFormattedAndResetClearsIt)
{
    SoundPlayer sound;
    sound.setEnabled(false);
    GameScreen screen(sound);
    auto* label = screen.findChild<QLabel*>("panelHeader");
    ASSERT_NE(label, nullptr);

    screen.setScore(1500);
    EXPECT_EQ(label->text(), "1.5k");

    screen.reset();
    EXPECT_EQ(label->text(), "0");
}

TEST_P(DirectionKeyTest, KeyHidesTheGuideAndResetRestoresIt)
{
    SoundPlayer sound;
    sound.setEnabled(false);
    GameScreen screen(sound);
    GuideWidget* guide = findGuide(screen);
    ASSERT_NE(guide, nullptr);
    EXPECT_FALSE(guide->isHidden());

    QTest::keyClick(&screen, static_cast<Qt::Key>(GetParam()));
    EXPECT_TRUE(guide->isHidden());

    screen.reset();
    EXPECT_FALSE(guide->isHidden());
}

INSTANTIATE_TEST_SUITE_P(Keys, DirectionKeyTest,
                         ::testing::Values(Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_Up, Qt::Key_Left,
                                           Qt::Key_Down, Qt::Key_Right));