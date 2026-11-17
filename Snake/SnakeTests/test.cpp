#include "MockInterfaces.h"
#include "app/ProfileStore.h"
#include "app/SoundPlayer.h"
#include "screens/MainMenuScreen.h"
#include "screens/SettingsScreen.h"
#include "screens/ShopScreen.h"
#include "screens/GameScreen.h"
#include "screens/PauseOverlay.h"
#include "widgets/OptionRow.h"
#include "widgets/OptionButton.h"
#include "widgets/GuideWidget.h"
#include <QAbstractButton>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QTemporaryDir>
#include <QTest>
#include <algorithm>
using ::testing::StrictMock;
namespace {
QAbstractButton* button(QWidget& w, const QString& text) {
    for (auto* b : w.findChildren<QAbstractButton*>()) if (b->text() == text) return b;
    return nullptr;
}
QList<OptionButton*> cells(OptionRow& row) {
    QList<OptionButton*> result;
    for (auto* b : row.findChildren<QAbstractButton*>()) if (auto* c = dynamic_cast<OptionButton*>(b)) result.append(c);
    return result;
}
QList<OptionSpec> options() {
    return {{QStringLiteral("drumstick"), "First"}, {QStringLiteral("trophy"), "Second"}, {QStringLiteral("sphere"), "Third", true, "earn it"}};
}
}
class NavigationTest : public ::testing::TestWithParam<Screen> {};
TEST_P(NavigationTest, NavigatesAndEmitsMatchingScreen) {
    QStackedWidget stack; ScreenNavigator nav(&stack); auto* content = new QWidget;
    nav.addScreen(GetParam(), content); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&nav, &ScreenNavigator::screenChanged, &nav, [&](Screen s) { receiver.screen(s); });
    EXPECT_CALL(receiver, screen(GetParam())); nav.goTo(GetParam());
    EXPECT_EQ(nav.current(), GetParam()); EXPECT_EQ(content->parentWidget(), stack.currentWidget());
}
INSTANTIATE_TEST_SUITE_P(AllScreens, NavigationTest, ::testing::Values(Screen::MainMenu, Screen::Settings, Screen::Achievements, Screen::Shop, Screen::Game));
TEST(Navigation, UnknownScreenDoesNotEmitOrChangePage) {
    QStackedWidget stack; ScreenNavigator nav(&stack); nav.addScreen(Screen::MainMenu, new QWidget);
    StrictMock<MockUiReceiver> receiver;
    QObject::connect(&nav, &ScreenNavigator::screenChanged, &nav, [&](Screen s) { receiver.screen(s); });
    nav.goTo(Screen::Shop); EXPECT_EQ(nav.current(), Screen::MainMenu); EXPECT_EQ(stack.currentIndex(), 0);
}
TEST(Navigation, FillingPageReservesHudSpace) {
    QStackedWidget stack; ScreenNavigator nav(&stack); nav.addScreen(Screen::Game, new QWidget, true);
    EXPECT_EQ(stack.widget(0)->layout()->contentsMargins().top(), 72);
}
class MenuTest : public ::testing::TestWithParam<int> {};
TEST_P(MenuTest, ClickEmitsOnlyMatchingRequest) {
    MainMenuScreen menu; StrictMock<MockUiReceiver> play, settings, achievements;
    QObject::connect(&menu, &MainMenuScreen::playRequested, &menu, [&] { play.action(); });
    QObject::connect(&menu, &MainMenuScreen::settingsRequested, &menu, [&] { settings.action(); });
    QObject::connect(&menu, &MainMenuScreen::achievementsRequested, &menu, [&] { achievements.action(); });
    auto* b = button(menu, QStringList{"Play", "Settings", "Achievements"}[GetParam()]); ASSERT_NE(b, nullptr);
    auto& expected = GetParam() == 0 ? play : GetParam() == 1 ? settings : achievements;
    EXPECT_CALL(expected, action()); b->click();
}
INSTANTIATE_TEST_SUITE_P(Actions, MenuTest, ::testing::Values(0, 1, 2));
TEST(OptionRow, InitialSelectionAndLockState) {
    OptionRow row(options(), {52, 52}, 32, 1); auto list = cells(row); ASSERT_EQ(list.size(), 3);
    EXPECT_FALSE(list[0]->isChecked()); EXPECT_TRUE(list[1]->isChecked()); EXPECT_FALSE(list[2]->isEnabled());
    EXPECT_EQ(list[2]->toolTip(), "Third (earn it)");
}
TEST(OptionRow, SelectUpdatesExclusiveSelectionAndEmits) {
    OptionRow row(options(), {52, 52}, 32); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&row, &OptionRow::selected, &row, [&](int i) { receiver.index(i); });
    EXPECT_CALL(receiver, index(1)); row.select(1);
    EXPECT_TRUE(cells(row)[1]->isChecked()); EXPECT_FALSE(cells(row)[0]->isChecked());
}
class InvalidSelectionTest : public ::testing::TestWithParam<int> {};
TEST_P(InvalidSelectionTest, InvalidOrLockedIndexDoesNotEmit) {
    OptionRow row(options(), {52, 52}, 32); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&row, &OptionRow::selected, &row, [&](int i) { receiver.index(i); });
    row.select(GetParam()); EXPECT_TRUE(cells(row)[0]->isChecked());
}
INSTANTIATE_TEST_SUITE_P(RejectedIndices, InvalidSelectionTest, ::testing::Values(-1, 2, 3, 100));
TEST(OptionRow, UnlockRestoresTooltipCursorAndSelection) {
    OptionRow row(options(), {52, 52}, 32); row.setLocked(2, false); auto* cell = cells(row)[2];
    EXPECT_TRUE(cell->isEnabled()); EXPECT_EQ(cell->toolTip(), "Third"); EXPECT_EQ(cell->cursor().shape(), Qt::PointingHandCursor);
    row.select(2); EXPECT_TRUE(cell->isChecked()); row.setLocked(2, true); EXPECT_EQ(cell->cursor().shape(), Qt::ArrowCursor);
    row.setLocked(-1, false); row.setLocked(100, false);
}
TEST(OptionRow, DisabledCellDoesNotEmitOnClick) {
    OptionRow row(options(), {52, 52}, 32); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&row, &OptionRow::selected, &row, [&](int i) { receiver.index(i); }); cells(row)[2]->click();
}
TEST(OptionRow, ClickEmitsMatchingIndex) {
    OptionRow row(options(), {52, 52}, 32); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&row, &OptionRow::selected, &row, [&](int i) { receiver.index(i); });
    EXPECT_CALL(receiver, index(1)); cells(row)[1]->click();
}
TEST(OptionRow, RandomSelectUsesOnlyUnlockedCandidate) {
    OptionRow row(options(), {52, 52}, 32); row.setLocked(0, true); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&row, &OptionRow::selected, &row, [&](int i) { receiver.index(i); });
    EXPECT_CALL(receiver, index(1)).Times(20); for (int i = 0; i < 20; ++i) row.selectRandomUnlocked();
}
TEST(OptionRow, RandomSelectWithAllLockedDoesNothing) {
    OptionRow row(options(), {52, 52}, 32); row.setLocked(0, true); row.setLocked(1, true);
    StrictMock<MockUiReceiver> receiver;
    QObject::connect(&row, &OptionRow::selected, &row, [&](int i) { receiver.index(i); }); row.selectRandomUnlocked();
}
TEST(OptionRow, SelectingOffscreenCellMakesItVisible) {
    OptionRow row(options(), {52, 52}, 32, 0, 1);
    EXPECT_FALSE(cells(row)[0]->isHidden()); EXPECT_TRUE(cells(row)[1]->isHidden()); row.select(1);
    EXPECT_FALSE(cells(row)[1]->isHidden()); EXPECT_TRUE(cells(row)[0]->isHidden());
}
class SettingsTest : public ::testing::TestWithParam<int> {};
TEST_P(SettingsTest, FruitCountSignalUsesCatalogValue) {
    QTemporaryDir dir; ProfileStore profile(dir.filePath("profile.json")); SettingsScreen screen(profile.data());
    auto rows = screen.findChildren<OptionRow*>(); ASSERT_EQ(rows.size(), 7); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&screen, &SettingsScreen::fruitCountSelected, &screen, [&](int i) { receiver.index(i); });
    EXPECT_CALL(receiver, index(Catalog::kFruitCounts[GetParam()])); rows[2]->select(GetParam());
}
INSTANTIATE_TEST_SUITE_P(FruitCounts, SettingsTest, ::testing::Values(0, 1, 2, 3));
class SettingsChoiceTest : public ::testing::TestWithParam<int> {};
TEST_P(SettingsChoiceTest, SpeedAndBoardSignalsUseCatalogValues) {
    QTemporaryDir dir; ProfileStore profile(dir.filePath("profile.json")); SettingsScreen screen(profile.data());
    auto rows = screen.findChildren<OptionRow*>(); StrictMock<MockUiReceiver> speed, board;
    QObject::connect(&screen, &SettingsScreen::speedSelected, &screen, [&](const QString& s) { speed.text(s); });
    QObject::connect(&screen, &SettingsScreen::boardSizeSelected, &screen, [&](const QString& s) { board.text(s); });
    EXPECT_CALL(speed, text(QString(Catalog::kSpeeds[GetParam()]))); EXPECT_CALL(board, text(QString(Catalog::kBoardSizes[GetParam()])));
    rows[3]->select(GetParam()); rows[4]->select(GetParam());
}
INSTANTIATE_TEST_SUITE_P(Choices, SettingsChoiceTest, ::testing::Values(0, 1, 2));
TEST(Settings, BackEmitsRequest) {
    QTemporaryDir dir; ProfileStore profile(dir.filePath("profile.json")); SettingsScreen screen(profile.data()); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&screen, &PanelScreen::backRequested, &screen, [&] { receiver.action(); });
    auto* back = button(screen, "Back"); ASSERT_NE(back, nullptr); EXPECT_CALL(receiver, action()); back->click();
}
TEST(Settings, UnlockColorEnablesCellAndEmitsCorrectId) {
    QTemporaryDir dir; ProfileStore profile(dir.filePath("profile.json")); SettingsScreen screen(profile.data());
    auto rows = screen.findChildren<OptionRow*>();
    for (auto kind : kColorKinds) {
        auto catalog = Catalog::colors(kind);
        for (int i = 0; i < static_cast<int>(catalog.size()); ++i) {
            const QString id = QLatin1StringView(catalog[i].id); screen.unlockColor(kind, id); StrictMock<MockUiReceiver> receiver;
            auto c = QObject::connect(&screen, &SettingsScreen::colorSelected, &screen, [&](ColorKind k, const QString& s) { receiver.color(k, s); });
            EXPECT_CALL(receiver, color(kind, id)); rows[5 + static_cast<int>(kind)]->select(i); QObject::disconnect(c);
        }
    }
}
TEST(Shop, UnaffordableButtonsAreDisabled) {
    ProfileData profile; ShopScreen screen(profile); auto buys = screen.findChildren<QPushButton*>("buyButton"); ASSERT_FALSE(buys.isEmpty());
    for (auto* buy : buys) EXPECT_FALSE(buy->isEnabled());
}
TEST(Shop, PurchasesEmitCorrectIdsInAscendingPriceOrder) {
    ProfileData profile; profile.applesBalance = 1000000; ShopScreen screen(profile);
    struct Item { ColorKind kind; QString id; int price; }; std::vector<Item> items;
    for (auto kind : kColorKinds) for (const auto& color : Catalog::colors(kind))
        if (color.source == ColorSource::Shop) items.push_back({kind, QLatin1StringView(color.id), color.price});
    std::stable_sort(items.begin(), items.end(), [](const Item& a, const Item& b) { return a.price < b.price; });
    auto buys = screen.findChildren<QPushButton*>("buyButton"); ASSERT_EQ(buys.size(), items.size()); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&screen, &ShopScreen::purchaseRequested, &screen, [&](ColorKind k, const QString& s) { receiver.color(k, s); });
    for (int i = 0; i < buys.size(); ++i) { EXPECT_CALL(receiver, color(items[i].kind, items[i].id)); buys[i]->click(); }
}
TEST(Shop, RefreshRemovesOwnedItemsWithoutDuplicates) {
    ProfileData profile; ShopScreen screen(profile);
    for (auto kind : kColorKinds) for (const auto& color : Catalog::colors(kind)) profile.unlocked(kind).append(QLatin1StringView(color.id));
    screen.refresh(profile); screen.refresh(profile); EXPECT_TRUE(screen.findChildren<QPushButton*>("buyButton").isEmpty());
    int messages = 0; for (auto* label : screen.findChildren<QLabel*>()) if (label->text() == "Everything is unlocked!") ++messages;
    EXPECT_EQ(messages, 1);
}
TEST(Game, EscapeRequestsPause) {
    SoundPlayer sound; sound.setEnabled(false); GameScreen screen(sound); StrictMock<MockUiReceiver> receiver;
    QObject::connect(&screen, &GameScreen::pauseRequested, &screen, [&] { receiver.action(); }); EXPECT_CALL(receiver, action()); QTest::keyClick(&screen, Qt::Key_Escape);
}
TEST(Game, ScoreFormatsAndResetClearsIt) {
    SoundPlayer sound; sound.setEnabled(false); GameScreen screen(sound); auto* label = screen.findChild<QLabel*>("panelHeader"); ASSERT_NE(label, nullptr);
    screen.setScore(1500); EXPECT_EQ(label->text(), "1.5k"); screen.reset(); EXPECT_EQ(label->text(), "0");
}
class DirectionTest : public ::testing::TestWithParam<int> {};
TEST_P(DirectionTest, DirectionHidesGuideAndResetRestoresIt) {
    SoundPlayer sound; sound.setEnabled(false); GameScreen screen(sound); GuideWidget* guide = nullptr;
    for (auto* w : screen.findChildren<QWidget*>()) if (auto* g = dynamic_cast<GuideWidget*>(w)) guide = g;
    ASSERT_NE(guide, nullptr); EXPECT_FALSE(guide->isHidden()); QTest::keyClick(&screen, static_cast<Qt::Key>(GetParam()));
    EXPECT_TRUE(guide->isHidden()); screen.reset(); EXPECT_FALSE(guide->isHidden());
}
INSTANTIATE_TEST_SUITE_P(Keys, DirectionTest, ::testing::Values(Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_Up, Qt::Key_Left, Qt::Key_Down, Qt::Key_Right));
TEST(Pause, EscapeEmitsResumeAndSummaryContainsSettings) {
    SoundPlayer sound; sound.setEnabled(false); PauseOverlay screen(sound); StrictMock<MockUiReceiver> receiver;
    EXPECT_TRUE(screen.isHidden()); screen.setSummary("Classic", "Fast", "Large"); auto* summary = screen.findChild<QLabel*>("panelText"); ASSERT_NE(summary, nullptr);
    EXPECT_TRUE(summary->text().contains("Classic")); EXPECT_TRUE(summary->text().contains("Fast")); EXPECT_TRUE(summary->text().contains("Large"));
    QObject::connect(&screen, &PauseOverlay::resumeRequested, &screen, [&] { receiver.action(); }); EXPECT_CALL(receiver, action()); QTest::keyClick(&screen, Qt::Key_Escape);
}
