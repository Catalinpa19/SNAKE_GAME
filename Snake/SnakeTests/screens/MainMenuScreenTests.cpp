#include "screens/MainMenuScreen.h"

#include "support/TestHelpers.h"
#include "widgets/MenuHeaderWidget.h"

#include <gtest/gtest.h>

namespace
{
struct Requests
{
    int play = 0;
    int settings = 0;
    int achievements = 0;
};

class MenuButtonTest : public ::testing::TestWithParam<int>
{
};
} // namespace

TEST_P(MenuButtonTest, ClickEmitsOnlyMatchingRequest)
{
    MainMenuScreen menu;
    Requests requests;
    QObject::connect(&menu, &MainMenuScreen::playRequested, &menu, [&] { ++requests.play; });
    QObject::connect(&menu, &MainMenuScreen::settingsRequested, &menu, [&] { ++requests.settings; });
    QObject::connect(&menu, &MainMenuScreen::achievementsRequested, &menu, [&] { ++requests.achievements; });

    auto* button = findButton(menu, QStringList{"Play", "Settings", "Achievements"}[GetParam()]);
    ASSERT_NE(button, nullptr);
    button->click();

    EXPECT_EQ(requests.play, GetParam() == 0 ? 1 : 0);
    EXPECT_EQ(requests.settings, GetParam() == 1 ? 1 : 0);
    EXPECT_EQ(requests.achievements, GetParam() == 2 ? 1 : 0);
}

INSTANTIATE_TEST_SUITE_P(Buttons, MenuButtonTest, ::testing::Values(0, 1, 2));

TEST(MainMenu, ExposesItsHeader)
{
    MainMenuScreen menu;
    EXPECT_NE(menu.header(), nullptr);
}