#include <QApplication>
#include "mainwindow.h"

using namespace TowFATool;

int main(int argc, char* argv[])
{
    QApplication app{argc, argv};
    MainWindow window;
    window.show();
    return app.exec();
}