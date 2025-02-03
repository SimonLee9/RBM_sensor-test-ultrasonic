#include "radarwidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QDebug>

RadarWidget::RadarWidget(QWidget *parent)
    : QWidget(parent)
    , m_sensor1Dist(0)
    , m_sensor2Dist(0)
    , m_threshold(500) // 기본 임계거리 500mm (0.5m)
{
    setMinimumSize(300, 200);
}

void RadarWidget::setSensor1Distance(int dist)
{
    if (dist != m_sensor1Dist) {
        m_sensor1Dist = dist;
        update(); // 값 갱신 시 다시 그림 요청
    }
}

void RadarWidget::setSensor2Distance(int dist)
{
    if (dist != m_sensor2Dist) {
        m_sensor2Dist = dist;
        update();
    }
}

void RadarWidget::setThreshold(int threshold)
{
    if (threshold != m_threshold) {
        m_threshold = threshold;
        update();
    }
}

void RadarWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 전체 배경을 검은색으로 칠함
    painter.fillRect(rect(), Qt::black);

    int w = width();
    int h = height();

    // 위젯을 좌/우 영역으로 나눔
    int halfWidth = w / 2;
    QRect leftRect(0, 0, halfWidth, h);
    QRect rightRect(halfWidth, 0, halfWidth, h);

    // 좌측 센서: m_sensor1Dist 값이 임계값 미만이면 장애물 감지로 판단
    bool leftObstacle = (m_sensor1Dist > 0 && m_sensor1Dist < m_threshold);
    // 우측 센서: m_sensor2Dist 값이 임계값 미만이면 장애물 감지로 판단
    bool rightObstacle = (m_sensor2Dist > 0 && m_sensor2Dist < m_threshold);

    // 좌측 영역 그리기
    QString leftText = QString("Left: %1 mm").arg(m_sensor1Dist);
    if (leftObstacle) {
        // 임계값 이하 → 빨간색 영역 및 경고 메시지
        painter.fillRect(leftRect, Qt::red);
        leftText.append(" - Obstacle!");
    } else {
        // 임계값 이상(또는 값이 없으면) → 초록색 영역
        painter.fillRect(leftRect, Qt::darkGreen);
    }
    painter.setPen(Qt::white);
    painter.drawText(leftRect, Qt::AlignCenter, leftText);

    // 우측 영역 그리기
    QString rightText = QString("Right: %1 mm").arg(m_sensor2Dist);
    if (rightObstacle) {
        painter.fillRect(rightRect, Qt::red);
        rightText.append(" - Obstacle!");
    } else {
        painter.fillRect(rightRect, Qt::darkGreen);
    }
    painter.drawText(rightRect, Qt::AlignCenter, rightText);
}
