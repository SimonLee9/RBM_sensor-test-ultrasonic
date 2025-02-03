#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "radarwidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial1(new QSerialPort(this))
    , serial2(new QSerialPort(this))
{
    ui->setupUi(this);

    // RadarWidget 생성하여 centralWidget으로 설정
    m_radarWidget = new RadarWidget(this);
    setCentralWidget(m_radarWidget);

    // -----------------------------
    // (1) 센서1 포트 설정/오픈
    // -----------------------------
    serial1->setPortName("/dev/ttyUSB0"); // Windows 예시
    serial1->setBaudRate(QSerialPort::Baud9600);
    //serial1->setBaudRate(QSerialPort::Baud115200);
    serial1->setDataBits(QSerialPort::Data8);
    serial1->setParity(QSerialPort::NoParity);
    serial1->setStopBits(QSerialPort::OneStop);
    serial1->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial1->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serial1->portName();
    } else {
        qDebug() << "Serial1 opened:" << serial1->portName();
        connect(serial1, &QSerialPort::readyRead, this, &MainWindow::readDataSensor1);
    }
    m_buffer1.clear();

    // -----------------------------
    // (2) 센서2 포트 설정/오픈
    // -----------------------------
    serial2->setPortName("/dev/ttyUSB1"); // Windows 예시
    serial2->setBaudRate(QSerialPort::Baud9600);
    //serial2->setBaudRate(QSerialPort::Baud115200);
    serial2->setDataBits(QSerialPort::Data8);
    serial2->setParity(QSerialPort::NoParity);
    serial2->setStopBits(QSerialPort::OneStop);
    serial2->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial2->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serial2->portName();
    } else {
        qDebug() << "Serial2 opened:" << serial2->portName();
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

// -----------------------------------------
// 센서1 데이터 수신 + 파싱
// -----------------------------------------
void MainWindow::readDataSensor1()
{
    QByteArray data = serial1->readAll();
    m_buffer1.append(data);

    // A02YYUW: [0xFF][High][Low][Checksum] (4바이트)
    while (m_buffer1.size() >= 4) {
        if (static_cast<unsigned char>(m_buffer1.at(0)) == 0xFF) {
            QByteArray packet = m_buffer1.left(4);
            unsigned char startByte = static_cast<unsigned char>(packet.at(0));
            unsigned char highByte  = static_cast<unsigned char>(packet.at(1));
            unsigned char lowByte   = static_cast<unsigned char>(packet.at(2));
            unsigned char checkSum  = static_cast<unsigned char>(packet.at(3));

            unsigned char calcSum = (startByte + highByte + lowByte) & 0xFF;
            if (calcSum == checkSum) {
                // 유효 거리(mm)
                int distance = (highByte << 8) | lowByte;

                // -> RadarWidget에 전달
                m_radarWidget->setSensor1Distance(distance);

                // 디버그 출력(선택)
                qDebug() << "[Sensor1]" << distance << "mm";

                // 버퍼에서 4바이트 제거
                m_buffer1.remove(0, 4);
            } else {
                // 패킷 손상 -> 1바이트씩 버퍼 제거
                m_buffer1.remove(0, 1);
            }
        } else {
            m_buffer1.remove(0, 1);
        }
    }
}

// -----------------------------------------
// 센서2 데이터 수신 + 파싱
// -----------------------------------------
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

                m_radarWidget->setSensor2Distance(distance);

                qDebug() << "[Sensor2]" << distance << "mm";

                m_buffer2.remove(0, 4);
            } else {
                m_buffer2.remove(0, 1);
            }
        } else {
            m_buffer2.remove(0, 1);
        }
    }
}
