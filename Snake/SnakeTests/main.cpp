#include <QApplication>
#include <gmock/gmock.h>
int main(int argc, char** argv) {
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("SnakeUiTests");
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
