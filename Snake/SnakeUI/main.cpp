#include "app/MainWindow.h"
#include "app/Theme.h"

#include <QApplication>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QtSvg/QSvgRenderer>

namespace
{
QIcon makeAppIcon()
{
    QSvgRenderer renderer(QStringLiteral(":/art/drumstick.svg"));
    QIcon icon;
    for (const int size : {16, 32, 48, 64, 128, 256})
    {
        QPixmap pixmap(size, size);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        renderer.render(&painter, QRectF(0, 0, size, size));
        icon.addPixmap(pixmap);
    }
    return icon;
}
} // namespace

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Snake"));
    app.setWindowIcon(makeAppIcon());
    app.setStyleSheet(Theme::styleSheet());

    // LOGIC: createServices(profilePath) from Factory.h goes here. The profile path is the AppData folder
    // (QStandardPaths::AppDataLocation): SnakeLogic must not compute it. The services are then given to MainWindow.
    MainWindow window;
    window.show();
    return app.exec();
}
