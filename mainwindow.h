#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>

class IntegratedSensorWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // DFROBOT 센서 (4바이트 패킷)
    void readDFLeftFront1();
    void readDFLeftFront2();
    void readDFRightFront1();
    void readDFRightFront2();

    // Nova‑A4 센서 (9바이트 패킷)
    void readNovaTop();
    void readNovaRight();
    void readNovaBottom();
    void readNovaLeft();

private:
    // DFROBOT 센서용 시리얼 포트 및 버퍼
    QSerialPort *serialDFLeftFront1;
    QSerialPort *serialDFLeftFront2;
    QSerialPort *serialDFRightFront1;
    QSerialPort *serialDFRightFront2;
    QByteArray bufferDFLeftFront1;
    QByteArray bufferDFLeftFront2;
    QByteArray bufferDFRightFront1;
    QByteArray bufferDFRightFront2;

    // Nova‑A4 센서용 시리얼 포트 및 버퍼
    QSerialPort *serialNovaTop;
    QSerialPort *serialNovaRight;
    QSerialPort *serialNovaBottom;
    QSerialPort *serialNovaLeft;
    QByteArray bufferNovaTop;
    QByteArray bufferNovaRight;
    QByteArray bufferNovaBottom;
    QByteArray bufferNovaLeft;

    IntegratedSensorWidget *integratedWidget;
};

#endif // MAINWINDOW_H
