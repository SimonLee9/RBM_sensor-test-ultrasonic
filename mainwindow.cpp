#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
{
    ui->setupUi(this);

    // (1) 시리얼포트 설정
    // 예: Windows라면 "COM3", Linux라면 "/dev/ttyUSB0" 등
    serial->setPortName("/dev/ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // (2) 포트 오픈
    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serial->portName();
        return;
    } else {
        qDebug() << "Serial port opened:" << serial->portName();
    }

    // (3) readyRead 시그널 -> readData 슬롯 연결
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    // 버퍼 초기화
    m_buffer.clear();
}

MainWindow::~MainWindow()
{
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;
}

// (4) 데이터 수신 및 파싱
void MainWindow::readData()
{
    // 들어온 데이터 추출
    QByteArray data = serial->readAll();
    m_buffer.append(data);

    // A02YYUW는 4바이트 패킷을 주기적으로 보냄
    // 패킷 구조: [0xFF][High][Low][Checksum]
    // 필요한 만큼 버퍼를 검사
    while (m_buffer.size() >= 4) {
        // 0xFF(패킷 시작)을 찾는다
        if (static_cast<unsigned char>(m_buffer.at(0)) == 0xFF) {
            // 패킷 4바이트 추출
            QByteArray packet = m_buffer.left(4);

            // 바이트 파싱
            unsigned char startByte = static_cast<unsigned char>(packet.at(0));
            unsigned char highByte  = static_cast<unsigned char>(packet.at(1));
            unsigned char lowByte   = static_cast<unsigned char>(packet.at(2));
            unsigned char checkSum  = static_cast<unsigned char>(packet.at(3));

            // Checksum 확인
            unsigned char calcSum = (startByte + highByte + lowByte) & 0xFF;
            if (calcSum == checkSum) {
                // 유효한 데이터라면 거리 계산 (단위: mm)
                int distance = (highByte << 8) | lowByte;
                qDebug() << "Distance(mm):" << distance;
                // 추가 로직: UI에 표시, 로깅 등

                // 버퍼에서 해당 4바이트는 제거
                m_buffer.remove(0, 4);

            } else {
                // 잘못된 패킷이면 첫 바이트만 제거하고 다시 검사
                m_buffer.remove(0, 1);
            }
        } else {
            // 0xFF가 아니라면 해당 바이트 버림
            m_buffer.remove(0, 1);
        }
    }
}
