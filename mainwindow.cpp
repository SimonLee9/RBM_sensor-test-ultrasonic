#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial1(new QSerialPort(this))
    , serial2(new QSerialPort(this))
    , sensor1Distance(-1)
    , sensor2Distance(-1)
{
    ui->setupUi(this);

    // --- (1) 센서1 시리얼 포트 설정/오픈 ---
    serial1->setPortName("/dev/ttyUSB0"); // 예: Windows에서는 COM3
    serial1->setBaudRate(QSerialPort::Baud9600);
    serial1->setDataBits(QSerialPort::Data8);
    serial1->setParity(QSerialPort::NoParity);
    serial1->setStopBits(QSerialPort::OneStop);
    serial1->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial1->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serial1->portName();
    } else {
        qDebug() << "Serial port 1 opened:" << serial1->portName();
        connect(serial1, &QSerialPort::readyRead, this, &MainWindow::readDataSensor1);
    }
    m_buffer1.clear();

    // --- (2) 센서2 시리얼 포트 설정/오픈 ---
    serial2->setPortName("/dev/ttyUSB1"); // 예: Windows에서는 COM4
    serial2->setBaudRate(QSerialPort::Baud9600);
    serial2->setDataBits(QSerialPort::Data8);
    serial2->setParity(QSerialPort::NoParity);
    serial2->setStopBits(QSerialPort::OneStop);
    serial2->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial2->open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open port" << serial2->portName();
    } else {
        qDebug() << "Serial port 2 opened:" << serial2->portName();
        connect(serial2, &QSerialPort::readyRead, this, &MainWindow::readDataSensor2);
    }
    m_buffer2.clear();
}

MainWindow::~MainWindow()
{
    if (serial1->isOpen()) serial1->close();
    if (serial2->isOpen()) serial2->close();
    delete ui;
}

// ---------------------------------------------------------------------------------
// (A) 센서1 데이터 수신
// ---------------------------------------------------------------------------------
void MainWindow::readDataSensor1()
{
    QByteArray data = serial1->readAll();
    m_buffer1.append(data);

    while (m_buffer1.size() >= 4) {
        if (static_cast<unsigned char>(m_buffer1.at(0)) == 0xFF) {
            QByteArray packet = m_buffer1.left(4);
            unsigned char startByte = static_cast<unsigned char>(packet.at(0));
            unsigned char highByte  = static_cast<unsigned char>(packet.at(1));
            unsigned char lowByte   = static_cast<unsigned char>(packet.at(2));
            unsigned char checkSum  = static_cast<unsigned char>(packet.at(3));

            unsigned char calcSum = (startByte + highByte + lowByte) & 0xFF;
            if (calcSum == checkSum) {
                // 거리(mm) 계산
                int distance = (highByte << 8) | lowByte;
                // --- 센서1 거리 업데이트 ---
                sensor1Distance = distance;

                // --- 한 줄에 출력하기 ---
                qDebug() << "[Sensor1]" << sensor1Distance << "mm |"
                         << "[Sensor2]" << sensor2Distance << "mm";

                // 정상 패킷이므로 버퍼에서 제거
                m_buffer1.remove(0, 4);
            } else {
                m_buffer1.remove(0, 1);
            }
        } else {
            m_buffer1.remove(0, 1);
        }
    }
}

// ---------------------------------------------------------------------------------
// (B) 센서2 데이터 수신
// ---------------------------------------------------------------------------------
void MainWindow::readDataSensor2()
{
    QByteArray data = serial2->readAll();
    m_buffer2.append(data);

    while (m_buffer2.size() >= 4) {
        if (static_cast<unsigned char>(m_buffer2.at(0)) == 0xFF) {
            QByteArray packet = m_buffer2.left(4);
            unsigned char startByte = static_cast<unsigned char>(packet.at(0));
            unsigned char highByte  = static_cast<unsigned char>(packet.at(1));
            unsigned char lowByte   = static_cast<unsigned char>(packet.at(2));
            unsigned char checkSum  = static_cast<unsigned char>(packet.at(3));

            unsigned char calcSum = (startByte + highByte + lowByte) & 0xFF;
            if (calcSum == checkSum) {
                int distance = (highByte << 8) | lowByte;
                // --- 센서2 거리 업데이트 ---
                sensor2Distance = distance;

                // --- 한 줄에 출력하기 ---
                qDebug() << "[Sensor1]" << sensor1Distance << "mm |"
                         << "[Sensor2]" << sensor2Distance << "mm";

                m_buffer2.remove(0, 4);
            } else {
                m_buffer2.remove(0, 1);
            }
        } else {
            m_buffer2.remove(0, 1);
        }
    }
}
