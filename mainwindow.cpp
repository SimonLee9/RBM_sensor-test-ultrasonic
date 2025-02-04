#include "mainwindow.h"
#include "integratedsensorwidget.h"
#include <QDebug>

// 생성자: 각 센서별 시리얼 포트 초기화 및 연결
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      serialDFLeftFront1(new QSerialPort(this)),
      serialDFLeftFront2(new QSerialPort(this)),
      serialDFRightFront1(new QSerialPort(this)),
      serialDFRightFront2(new QSerialPort(this)),
      serialNovaTop(new QSerialPort(this)),
      serialNovaRight(new QSerialPort(this)),
      serialNovaBottom(new QSerialPort(this)),
      serialNovaLeft(new QSerialPort(this))
{
    integratedWidget = new IntegratedSensorWidget(this);
    setCentralWidget(integratedWidget);

    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // DFROBOT 센서 (보오드 9600bps)
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // DF Left Front1 (예: COM3)
    serialDFLeftFront1->setPortName("COM3");
    serialDFLeftFront1->setBaudRate(9600);
    serialDFLeftFront1->setDataBits(QSerialPort::Data8);
    serialDFLeftFront1->setParity(QSerialPort::NoParity);
    serialDFLeftFront1->setStopBits(QSerialPort::OneStop);
    serialDFLeftFront1->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialDFLeftFront1->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open DFLeftFront1";
    else
        connect(serialDFLeftFront1, &QSerialPort::readyRead, this, &MainWindow::readDFLeftFront1);
    bufferDFLeftFront1.clear();

    // DF Left Front2 (예: COM4)
    serialDFLeftFront2->setPortName("COM4");
    serialDFLeftFront2->setBaudRate(9600);
    serialDFLeftFront2->setDataBits(QSerialPort::Data8);
    serialDFLeftFront2->setParity(QSerialPort::NoParity);
    serialDFLeftFront2->setStopBits(QSerialPort::OneStop);
    serialDFLeftFront2->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialDFLeftFront2->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open DFLeftFront2";
    else
        connect(serialDFLeftFront2, &QSerialPort::readyRead, this, &MainWindow::readDFLeftFront2);
    bufferDFLeftFront2.clear();

    // DF Right Front1 (예: COM5)
    serialDFRightFront1->setPortName("/dev/ttyUSB5");
    serialDFRightFront1->setBaudRate(9600);
    serialDFRightFront1->setDataBits(QSerialPort::Data8);
    serialDFRightFront1->setParity(QSerialPort::NoParity);
    serialDFRightFront1->setStopBits(QSerialPort::OneStop);
    serialDFRightFront1->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialDFRightFront1->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open DFRightFront1";
    else
        connect(serialDFRightFront1, &QSerialPort::readyRead, this, &MainWindow::readDFRightFront1);
    bufferDFRightFront1.clear();

    // DF Right Front2 (예: COM6)
    serialDFRightFront2->setPortName("/dev/ttyUSB3");
    serialDFRightFront2->setBaudRate(9600);
    serialDFRightFront2->setDataBits(QSerialPort::Data8);
    serialDFRightFront2->setParity(QSerialPort::NoParity);
    serialDFRightFront2->setStopBits(QSerialPort::OneStop);
    serialDFRightFront2->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialDFRightFront2->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open DFRightFront2";
    else
        connect(serialDFRightFront2, &QSerialPort::readyRead, this, &MainWindow::readDFRightFront2);
    bufferDFRightFront2.clear();

    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Nova‑A4 센서 (보오드 230400bps)
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // NovaTop (예: COM7)
    serialNovaTop->setPortName("/dev/ttyUSB2");
    serialNovaTop->setBaudRate(230400);
    serialNovaTop->setDataBits(QSerialPort::Data8);
    serialNovaTop->setParity(QSerialPort::NoParity);
    serialNovaTop->setStopBits(QSerialPort::OneStop);
    serialNovaTop->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialNovaTop->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open NovaTop";
    else
        connect(serialNovaTop, &QSerialPort::readyRead, this, &MainWindow::readNovaTop);
    bufferNovaTop.clear();

    // NovaRight (예: COM8)
    serialNovaRight->setPortName("/dev/ttyUSB0");
    serialNovaRight->setBaudRate(230400);
    serialNovaRight->setDataBits(QSerialPort::Data8);
    serialNovaRight->setParity(QSerialPort::NoParity);
    serialNovaRight->setStopBits(QSerialPort::OneStop);
    serialNovaRight->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialNovaRight->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open NovaRight";
    else
        connect(serialNovaRight, &QSerialPort::readyRead, this, &MainWindow::readNovaRight);
    bufferNovaRight.clear();

    // NovaBottom (예: COM9)
    serialNovaBottom->setPortName("/dev/ttyUSB4");
    serialNovaBottom->setBaudRate(230400);
    serialNovaBottom->setDataBits(QSerialPort::Data8);
    serialNovaBottom->setParity(QSerialPort::NoParity);
    serialNovaBottom->setStopBits(QSerialPort::OneStop);
    serialNovaBottom->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialNovaBottom->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open NovaBottom";
    else
        connect(serialNovaBottom, &QSerialPort::readyRead, this, &MainWindow::readNovaBottom);
    bufferNovaBottom.clear();

    // NovaLeft (예: COM10)
    serialNovaLeft->setPortName("/dev/ttyUSB1");
    serialNovaLeft->setBaudRate(230400);
    serialNovaLeft->setDataBits(QSerialPort::Data8);
    serialNovaLeft->setParity(QSerialPort::NoParity);
    serialNovaLeft->setStopBits(QSerialPort::OneStop);
    serialNovaLeft->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialNovaLeft->open(QIODevice::ReadWrite))
        qDebug() << "Failed to open NovaLeft";
    else
        connect(serialNovaLeft, &QSerialPort::readyRead, this, &MainWindow::readNovaLeft);
    bufferNovaLeft.clear();
}

MainWindow::~MainWindow()
{
    if(serialDFLeftFront1->isOpen())   serialDFLeftFront1->close();
    if(serialDFLeftFront2->isOpen())   serialDFLeftFront2->close();
    if(serialDFRightFront1->isOpen())  serialDFRightFront1->close();
    if(serialDFRightFront2->isOpen())  serialDFRightFront2->close();
    if(serialNovaTop->isOpen())        serialNovaTop->close();
    if(serialNovaRight->isOpen())      serialNovaRight->close();
    if(serialNovaBottom->isOpen())     serialNovaBottom->close();
    if(serialNovaLeft->isOpen())       serialNovaLeft->close();
}

// ----- DFROBOT 센서: 4바이트 패킷 [0xFF, High, Low, Checksum] -----
// Checksum = (0xFF + High + Low) & 0xFF

void MainWindow::readDFLeftFront1()
{
    bufferDFLeftFront1.append(serialDFLeftFront1->readAll());
    while(bufferDFLeftFront1.size() >= 4) {
        if(static_cast<quint8>(bufferDFLeftFront1.at(0)) != 0xFF) {
            bufferDFLeftFront1.remove(0,1);
            continue;
        }
        QByteArray packet = bufferDFLeftFront1.left(4);
        quint8 start = static_cast<quint8>(packet.at(0));
        quint8 high  = static_cast<quint8>(packet.at(1));
        quint8 low   = static_cast<quint8>(packet.at(2));
        quint8 cs    = static_cast<quint8>(packet.at(3));
        quint8 calc  = (start + high + low) & 0xFF;
        if(calc == cs) {
            int distance = (high << 8) | low;
            qDebug() << "[DF LF1]" << distance << "mm";
            integratedWidget->setDFLeftFront1(distance);
            bufferDFLeftFront1.remove(0,4);
        } else {
            qDebug() << "[DF LF1] Checksum error.";
            bufferDFLeftFront1.remove(0,1);
        }
    }
}

void MainWindow::readDFLeftFront2()
{
    bufferDFLeftFront2.append(serialDFLeftFront2->readAll());
    while(bufferDFLeftFront2.size() >= 4) {
        if(static_cast<quint8>(bufferDFLeftFront2.at(0)) != 0xFF) {
            bufferDFLeftFront2.remove(0,1);
            continue;
        }
        QByteArray packet = bufferDFLeftFront2.left(4);
        quint8 start = static_cast<quint8>(packet.at(0));
        quint8 high  = static_cast<quint8>(packet.at(1));
        quint8 low   = static_cast<quint8>(packet.at(2));
        quint8 cs    = static_cast<quint8>(packet.at(3));
        quint8 calc  = (start + high + low) & 0xFF;
        if(calc == cs) {
            int distance = (high << 8) | low;
            qDebug() << "[DF LF2]" << distance << "mm";
            integratedWidget->setDFLeftFront2(distance);
            bufferDFLeftFront2.remove(0,4);
        } else {
            qDebug() << "[DF LF2] Checksum error.";
            bufferDFLeftFront2.remove(0,1);
        }
    }
}

void MainWindow::readDFRightFront1()
{
    bufferDFRightFront1.append(serialDFRightFront1->readAll());
    while(bufferDFRightFront1.size() >= 4) {
        if(static_cast<quint8>(bufferDFRightFront1.at(0)) != 0xFF) {
            bufferDFRightFront1.remove(0,1);
            continue;
        }
        QByteArray packet = bufferDFRightFront1.left(4);
        quint8 start = static_cast<quint8>(packet.at(0));
        quint8 high  = static_cast<quint8>(packet.at(1));
        quint8 low   = static_cast<quint8>(packet.at(2));
        quint8 cs    = static_cast<quint8>(packet.at(3));
        quint8 calc  = (start + high + low) & 0xFF;
        if(calc == cs) {
            int distance = (high << 8) | low;
            qDebug() << "[DF RF1]" << distance << "mm";
            integratedWidget->setDFRightFront1(distance);
            bufferDFRightFront1.remove(0,4);
        } else {
            qDebug() << "[DF RF1] Checksum error.";
            bufferDFRightFront1.remove(0,1);
        }
    }
}

void MainWindow::readDFRightFront2()
{
    bufferDFRightFront2.append(serialDFRightFront2->readAll());
    while(bufferDFRightFront2.size() >= 4) {
        if(static_cast<quint8>(bufferDFRightFront2.at(0)) != 0xFF) {
            bufferDFRightFront2.remove(0,1);
            continue;
        }
        QByteArray packet = bufferDFRightFront2.left(4);
        quint8 start = static_cast<quint8>(packet.at(0));
        quint8 high  = static_cast<quint8>(packet.at(1));
        quint8 low   = static_cast<quint8>(packet.at(2));
        quint8 cs    = static_cast<quint8>(packet.at(3));
        quint8 calc  = (start + high + low) & 0xFF;
        if(calc == cs) {
            int distance = (high << 8) | low;
            qDebug() << "[DF RF2]" << distance << "mm";
            integratedWidget->setDFRightFront2(distance);
            bufferDFRightFront2.remove(0,4);
        } else {
            qDebug() << "[DF RF2] Checksum error.";
            bufferDFRightFront2.remove(0,1);
        }
    }
}

// ----- Nova‑A4 센서: 9바이트 패킷 [0xA5,0x01,x1,y1,x2,y2,x3,y3,cs] -----
// Checksum = (0xA5+0x01+x1+y1+x2+y2+x3+y3)&0xFF

void MainWindow::readNovaTop()
{
    bufferNovaTop.append(serialNovaTop->readAll());
    while(bufferNovaTop.size() >= 9) {
        if(static_cast<quint8>(bufferNovaTop.at(0)) != 0xA5) {
            bufferNovaTop.remove(0,1);
            continue;
        }
        QByteArray packet = bufferNovaTop.left(9);
        quint8 header = static_cast<quint8>(packet.at(0));
        quint8 id     = static_cast<quint8>(packet.at(1));
        quint8 x1     = static_cast<quint8>(packet.at(2));
        quint8 y1     = static_cast<quint8>(packet.at(3));
        quint8 x2     = static_cast<quint8>(packet.at(4));
        quint8 y2     = static_cast<quint8>(packet.at(5));
        quint8 x3     = static_cast<quint8>(packet.at(6));
        quint8 y3     = static_cast<quint8>(packet.at(7));
        quint8 cs     = static_cast<quint8>(packet.at(8));
        quint16 sum   = header + id + x1 + y1 + x2 + y2 + x3 + y3;
        quint8 expected = sum & 0xFF;
        if(expected == cs) {
            int distance = (y2 << 8) | x2;
            qDebug() << "[Nova Top]" << distance << "mm";
            integratedWidget->setNovaTop(distance);
            bufferNovaTop.remove(0,9);
        } else {
            qDebug() << "[Nova Top] Checksum error.";
            bufferNovaTop.remove(0,1);
        }
    }
}

void MainWindow::readNovaRight()
{
    bufferNovaRight.append(serialNovaRight->readAll());
    while(bufferNovaRight.size() >= 9) {
        if(static_cast<quint8>(bufferNovaRight.at(0)) != 0xA5) {
            bufferNovaRight.remove(0,1);
            continue;
        }
        QByteArray packet = bufferNovaRight.left(9);
        quint8 header = static_cast<quint8>(packet.at(0));
        quint8 id     = static_cast<quint8>(packet.at(1));
        quint8 x1     = static_cast<quint8>(packet.at(2));
        quint8 y1     = static_cast<quint8>(packet.at(3));
        quint8 x2     = static_cast<quint8>(packet.at(4));
        quint8 y2     = static_cast<quint8>(packet.at(5));
        quint8 x3     = static_cast<quint8>(packet.at(6));
        quint8 y3     = static_cast<quint8>(packet.at(7));
        quint8 cs     = static_cast<quint8>(packet.at(8));
        quint16 sum   = header + id + x1 + y1 + x2 + y2 + x3 + y3;
        quint8 expected = sum & 0xFF;
        if(expected == cs) {
            int distance = (y2 << 8) | x2;
            qDebug() << "[Nova Right]" << distance << "mm";
            integratedWidget->setNovaRight(distance);
            bufferNovaRight.remove(0,9);
        } else {
            qDebug() << "[Nova Right] Checksum error.";
            bufferNovaRight.remove(0,1);
        }
    }
}

void MainWindow::readNovaBottom()
{
    bufferNovaBottom.append(serialNovaBottom->readAll());
    while(bufferNovaBottom.size() >= 9) {
        if(static_cast<quint8>(bufferNovaBottom.at(0)) != 0xA5) {
            bufferNovaBottom.remove(0,1);
            continue;
        }
        QByteArray packet = bufferNovaBottom.left(9);
        quint8 header = static_cast<quint8>(packet.at(0));
        quint8 id     = static_cast<quint8>(packet.at(1));
        quint8 x1     = static_cast<quint8>(packet.at(2));
        quint8 y1     = static_cast<quint8>(packet.at(3));
        quint8 x2     = static_cast<quint8>(packet.at(4));
        quint8 y2     = static_cast<quint8>(packet.at(5));
        quint8 x3     = static_cast<quint8>(packet.at(6));
        quint8 y3     = static_cast<quint8>(packet.at(7));
        quint8 cs     = static_cast<quint8>(packet.at(8));
        quint16 sum   = header + id + x1 + y1 + x2 + y2 + x3 + y3;
        quint8 expected = sum & 0xFF;
        if(expected == cs) {
            int distance = (y2 << 8) | x2;
            qDebug() << "[Nova Bottom]" << distance << "mm";
            integratedWidget->setNovaBottom(distance);
            bufferNovaBottom.remove(0,9);
        } else {
            qDebug() << "[Nova Bottom] Checksum error.";
            bufferNovaBottom.remove(0,1);
        }
    }
}

void MainWindow::readNovaLeft()
{
    bufferNovaLeft.append(serialNovaLeft->readAll());
    while(bufferNovaLeft.size() >= 9) {
        if(static_cast<quint8>(bufferNovaLeft.at(0)) != 0xA5) {
            bufferNovaLeft.remove(0,1);
            continue;
        }
        QByteArray packet = bufferNovaLeft.left(9);
        quint8 header = static_cast<quint8>(packet.at(0));
        quint8 id     = static_cast<quint8>(packet.at(1));
        quint8 x1     = static_cast<quint8>(packet.at(2));
        quint8 y1     = static_cast<quint8>(packet.at(3));
        quint8 x2     = static_cast<quint8>(packet.at(4));
        quint8 y2     = static_cast<quint8>(packet.at(5));
        quint8 x3     = static_cast<quint8>(packet.at(6));
        quint8 y3     = static_cast<quint8>(packet.at(7));
        quint8 cs     = static_cast<quint8>(packet.at(8));
        quint16 sum   = header + id + x1 + y1 + x2 + y2 + x3 + y3;
        quint8 expected = sum & 0xFF;
        if(expected == cs) {
            int distance = (y2 << 8) | x2;
            qDebug() << "[Nova Left]" << distance << "mm";
            integratedWidget->setNovaLeft(distance);
            bufferNovaLeft.remove(0,9);
        } else {
            qDebug() << "[Nova Left] Checksum error.";
            bufferNovaLeft.remove(0,1);
        }
    }
}
