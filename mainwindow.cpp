#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // UI 구성: 중앙에 센서 데이터를 표시할 QLabel 생성
    label = new QLabel("Distance: N/A", this);
    label->setAlignment(Qt::AlignCenter);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(label);
    setCentralWidget(centralWidget);

    // 시리얼 포트 초기화
    serial = new QSerialPort(this);
    serial->setPortName("/dev/ttyUSB0"); // 실제 사용 포트명으로 수정 (예: "COM3" 또는 "/dev/ttyUSB0")

    // 보오드 레이트를 230400으로 설정 (정수값 사용)
    serial->setBaudRate(230400);

    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serial->portName();
    } else {
        qDebug() << "Serial port opened:" << serial->portName();
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSensorData);
    }
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
}

void MainWindow::readSensorData()
{
    // 수신된 데이터를 버퍼에 추가
    buffer.append(serial->readAll());

    // Nova-A4 센서의 패킷 길이는 9바이트
    while (buffer.size() >= 9) {
        // 패킷의 첫 바이트가 Header (0xA5)인지 확인
        if (static_cast<quint8>(buffer.at(0)) != 0xA5) {
            // 유효하지 않은 헤더면 한 바이트 제거하여 재동기화 시도
            buffer.remove(0, 1);
            continue;
        }

        // 9바이트 패킷 추출
        QByteArray packet = buffer.left(9);
        // 패킷 구성:
        // [0] Header    : 0xA5
        // [1] ID        : 0x01
        // [2] Reserve x1
        // [3] Reserve y1
        // [4] Distance x2 (고바이트)
        // [5] Distance y2 (저바이트)
        // [6] Peak x3
        // [7] Peak y3
        // [8] Checksum  : (0xA5 + 0x01 + x1 + y1 + x2 + y2 + x3 + y3) & 0xFF

        quint8 header    = static_cast<quint8>(packet.at(0));
        quint8 id        = static_cast<quint8>(packet.at(1));
        quint8 reserve_x = static_cast<quint8>(packet.at(2));
        quint8 reserve_y = static_cast<quint8>(packet.at(3));
        quint8 dist_x    = static_cast<quint8>(packet.at(4));
        quint8 dist_y    = static_cast<quint8>(packet.at(5));
        quint8 peak_x    = static_cast<quint8>(packet.at(6));
        quint8 peak_y    = static_cast<quint8>(packet.at(7));
        quint8 checksum  = static_cast<quint8>(packet.at(8));

        // 체크섬 계산 (하위 8비트)
        quint16 sum = header + id + reserve_x + reserve_y + dist_x + dist_y + peak_x + peak_y;
        quint8 expectedChecksum = sum & 0xFF;

        if (expectedChecksum == checksum) {
            // 유효한 패킷인 경우: 거리(mm) 계산 (Distance는 2바이트: 고바이트, 저바이트)
            int distance = (dist_y << 8) | dist_x;
            qDebug() << "Valid packet received. Distance:" << distance << "mm";
            label->setText(QString("Distance: %1 mm").arg(distance));

            // Reserve 및 Peak 값 출력 (필요 시)
            qDebug() << "Reserve:" << reserve_x << reserve_y
                     << "Peak:" << peak_x << peak_y;

            // 처리한 패킷은 버퍼에서 제거
            buffer.remove(0, 9);
        } else {
            qDebug() << "Checksum error: expected" << expectedChecksum << "got" << checksum;
            // 체크섬 불일치 시, 첫 바이트 제거하여 재동기화 시도
            buffer.remove(0, 1);
        }
    }
}
