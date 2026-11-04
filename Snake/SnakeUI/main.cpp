#include "SnakeUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SnakeUI window;
    window.show();
    return app.exec();
}
