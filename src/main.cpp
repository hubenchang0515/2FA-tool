#include <QApplication>
#include "mainwindow.h"

using namespace TowFATool;

int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app{argc, argv};
    MainWindow window;
    window.show();
    return app.exec();
}