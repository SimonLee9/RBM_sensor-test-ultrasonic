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

    // RadarWidget 생성 후 중앙 위젯으로 설정
    m_radarWidget = new RadarWidget(this);
    // (필요시 임계거리도 설정할 수 있음: 예를 들어 500mm)
    m_radarWidget->setThreshold(300); // 500
    setCentralWidget(m_radarWidget);

    // -----------------------------
    // (1) 센서1 시리얼 포트 설정/오픈 (좌측 센서)
    // -----------------------------
    serial1->setPortName("/dev/ttyUSB0"); // 예: Windows 환경, Linux는 "/dev/ttyUSB0" 등으로 수정
    serial1->setBaudRate(QSerialPort::Baud9600);
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
    // (2) 센서2 시리얼 포트 설정/오픈 (우측 센서)
    // -----------------------------
    serial2->setPortName("/dev/ttyUSB1"); // 예: Windows 환경, Linux는 "/dev/ttyUSB1" 등으로 수정
    serial2->setBaudRate(QSerialPort::Baud9600);
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
// 센서1 데이터 수신 및 파싱 (좌측 센서)
// -----------------------------------------
void MainWindow::readDataSensor1()
{
    QByteArray data = serial1->readAll();
    m_buffer1.append(data);

    // DFROBOT A02YYUW 패킷: [0xFF][High Byte][Low Byte][Checksum] (4바이트)
    while (m_buffer1.size() >= 4) {
        if (static_cast<unsigned char>(m_buffer1.at(0)) == 0xFF) {
            QByteArray packet = m_buffer1.left(4);
            unsigned char startByte = static_cast<unsigned char>(packet.at(0));
            unsigned char highByte  = static_cast<unsigned char>(packet.at(1));
            unsigned char lowByte   = static_cast<unsigned char>(packet.at(2));
            unsigned char checkSum  = static_cast<unsigned char>(packet.at(3));

            unsigned char calcSum = (startByte + highByte + lowByte) & 0xFF;
            if (calcSum == checkSum) {
                // 거리 (mm) 계산
                int distance = (highByte << 8) | lowByte;

                // 좌측 센서 데이터 업데이트
                m_radarWidget->setSensor1Distance(distance);

                qDebug() << "[Sensor1]" << distance << "mm";

                m_buffer1.remove(0, 4);
            } else {
                m_buffer1.remove(0, 1);
            }
        } else {
            m_buffer1.remove(0, 1);
        }
    }
}

// -----------------------------------------
// 센서2 데이터 수신 및 파싱 (우측 센서)
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

                // 우측 센서 데이터 업데이트
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
