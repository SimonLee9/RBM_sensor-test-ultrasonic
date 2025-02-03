#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Nova-A4 Sensor Test");
    window.resize(400, 300);
    window.show();
    //window.isHidden();
    return app.exec();
}
