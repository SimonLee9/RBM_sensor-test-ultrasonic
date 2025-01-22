#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readDataSensor1();
    void readDataSensor2();

private:
    Ui::MainWindow *ui;

    QSerialPort *serial1;
    QSerialPort *serial2;

    QByteArray m_buffer1;
    QByteArray m_buffer2;

    // ★ 센서1, 센서2의 마지막으로 읽은 거리값을 저장할 변수
    int sensor1Distance;
    int sensor2Distance;
};

#endif // MAINWINDOW_H
