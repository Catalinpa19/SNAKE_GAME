#include "screens/PauseOverlay.h"

#include "app/SoundPlayer.h"

#include <QLabel>
#include <QTest>

#include <gmock/gmock.h>

TEST(PauseOverlay, StartsHiddenAndSummaryShowsTheSettings)
{
    SoundPlayer sound;
    sound.setEnabled(false);
    PauseOverlay overlay(sound);
    EXPECT_TRUE(overlay.isHidden());

    overlay.setSummary("Classic", "Fast", "Large");

    auto* summary = overlay.findChild<QLabel*>("panelText");
    ASSERT_NE(summary, nullptr);
    EXPECT_THAT(summary->text().toStdString(),
                ::testing::AllOf(::testing::HasSubstr("Classic"), ::testing::HasSubstr("Fast"), ::testing::HasSubstr("Large")));
}

TEST(PauseOverlay, EscapeRequestsResume)
{
    SoundPlayer sound;
    sound.setEnabled(false);
    PauseOverlay overlay(sound);
    int requests = 0;
    QObject::connect(&overlay, &PauseOverlay::resumeRequested, &overlay, [&] { ++requests; });

    QTest::keyClick(&overlay, Qt::Key_Escape);

    EXPECT_EQ(requests, 1);
}