#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("통합 센서 시스템 (30cm 기준 경고)");
    window.resize(800, 800);
    window.show();
    return app.exec();
}
