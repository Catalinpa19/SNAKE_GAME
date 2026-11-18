#include "app/ScreenNavigator.h"
#include "screens/MainMenuScreen.h"

#include <QLayout>
#include <QStackedWidget>

#include <gmock/gmock.h>

#include <vector>

namespace
{
class NavigationTest : public ::testing::TestWithParam<Screen>
{
};
} // namespace

TEST_P(NavigationTest, NavigatesAndEmitsMatchingScreen)
{
    QStackedWidget stack;
    ScreenNavigator navigator(&stack);
    auto* content = new MainMenuScreen;
    navigator.addScreen(GetParam(), content);

    std::vector<Screen> changes;
    QObject::connect(&navigator, &ScreenNavigator::screenChanged, &navigator, [&](Screen screen) { changes.push_back(screen); });

    navigator.goTo(GetParam());

    EXPECT_THAT(changes, ::testing::ElementsAre(GetParam()));
    EXPECT_EQ(navigator.current(), GetParam());
    EXPECT_EQ(content->parentWidget(), stack.currentWidget());
}

INSTANTIATE_TEST_SUITE_P(AllScreens, NavigationTest,
                         ::testing::Values(Screen::MainMenu, Screen::Settings, Screen::Achievements, Screen::Shop, Screen::Game));

TEST(Navigation, UnknownScreenDoesNotEmitOrChangePage)
{
    QStackedWidget stack;
    ScreenNavigator navigator(&stack);
    navigator.addScreen(Screen::MainMenu, new MainMenuScreen);

    int changes = 0;
    QObject::connect(&navigator, &ScreenNavigator::screenChanged, &navigator, [&](Screen) { ++changes; });

    navigator.goTo(Screen::Shop);

    EXPECT_EQ(changes, 0);
    EXPECT_EQ(navigator.current(), Screen::MainMenu);
    EXPECT_EQ(stack.currentIndex(), 0);
}

TEST(Navigation, FillingPageReservesHudSpace)
{
    QStackedWidget stack;
    ScreenNavigator navigator(&stack);
    navigator.addScreen(Screen::Game, new MainMenuScreen, true);

    EXPECT_EQ(stack.widget(0)->layout()->contentsMargins().top(), 72);
}