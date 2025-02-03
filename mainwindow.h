#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>

class RadarWidget;

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

    // 시리얼 포트 2개
    QSerialPort *serial1;
    QSerialPort *serial2;

    // 버퍼
    QByteArray m_buffer1;
    QByteArray m_buffer2;

    // 센서 거리값을 시각화할 위젯
    RadarWidget *m_radarWidget;
};

#endif // MAINWINDOW_H
