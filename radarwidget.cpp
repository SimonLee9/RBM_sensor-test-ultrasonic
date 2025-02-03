#include "radarwidget.h"
#include <QPainter>
#include <QColor>
#include <QDebug>

RadarWidget::RadarWidget(QWidget *parent)
    : QWidget(parent)
    , m_sensor1Dist(0)
    , m_sensor2Dist(0)
    , m_maxDist(300) // 기본 300mm (30cm) 범위
{
    setMinimumSize(300, 200); // 위젯 최소 크기
}

void RadarWidget::setSensor1Distance(int dist)
{
    if (dist != m_sensor1Dist) {
        m_sensor1Dist = dist;
        update(); // 값이 바뀌면 다시 그리기
    }
}

void RadarWidget::setSensor2Distance(int dist)
{
    if (dist != m_sensor2Dist) {
        m_sensor2Dist = dist;
        update();
    }
}

void RadarWidget::setMaxDistance(int maxDist)
{
    if (m_maxDist != maxDist) {
        m_maxDist = maxDist;
        update();
    }
}

void RadarWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int w = width();
    int h = height();

    // 배경 색
    painter.fillRect(rect(), Qt::black);

    // 센서1, 센서2의 거리값이 0~m_maxDist 범위 안에 있다고 가정
    int dist1 = qBound(0, m_sensor1Dist, m_maxDist);
    int dist2 = qBound(0, m_sensor2Dist, m_maxDist);

    // 아래쪽을 기준으로 '반원' 형태로 센서1/2를 표시할 것이므로,
    // 아래 중앙이 (cx, cy) = (w/2, h)
    // 왼쪽 호는 센서1, 오른쪽 호는 센서2라고 가정하겠습니다.

    QPoint center(w/2, h);
    int radius = qMin(w/2, h); // 반지름(왼/오 반원 각각 최대)

    // --- (1) 바닥 그리드(원호) ---
    // 센서1/2 통합해서 5단계로 그려보기
    painter.setPen(QPen(Qt::gray, 1));
    for (int i = 1; i <= 5; ++i) {
        double rRatio = double(i)/5.0; // 0.2,0.4,0.6,0.8,1.0
        int r = int(radius * rRatio);

        // 왼쪽 반원
        QRect arcRectLeft(center.x() - r, center.y() - r, 2*r, 2*r);
        painter.drawArc(arcRectLeft, 180*16, 180*16); // 180도~360도 구간

        // 오른쪽 반원
        QRect arcRectRight(center.x() - r, center.y() - r, 2*r, 2*r);
        painter.drawArc(arcRectRight, 0, 180*16); // 0도~180도 구간
    }

    // --- (2) 센서1 반원(왼쪽) ---
    {
        double ratio = double(dist1)/m_maxDist; // 0~1
        int r = int(radius * ratio);

        // 가까울수록 빨강, 멀수록 초록(단순 보간)
        QColor nearColor(Qt::red);
        QColor farColor(Qt::green);
        int rr = nearColor.red()   + ratio*(farColor.red()   - nearColor.red());
        int rg = nearColor.green() + ratio*(farColor.green() - nearColor.green());
        int rb = nearColor.blue()  + ratio*(farColor.blue()  - nearColor.blue());
        QColor arcColor(rr, rg, rb);

        painter.setPen(QPen(arcColor, 3));
        QRect arcRectLeft(center.x() - r, center.y() - r, 2*r, 2*r);
        // 왼쪽 호: 180도~360도
        painter.drawArc(arcRectLeft, 180*16, 180*16);

        // 문자 표시
        painter.setPen(Qt::white);
        painter.drawText(center.x() - 80, center.y() - 10,
                         70, 20, Qt::AlignRight,
                         QString("S1: %1mm").arg(dist1));
    }

    // --- (3) 센서2 반원(오른쪽) ---
    {
        double ratio = double(dist2)/m_maxDist;
        int r = int(radius * ratio);

        QColor nearColor(Qt::red);
        QColor farColor(Qt::green);
        int rr = nearColor.red()   + ratio*(farColor.red()   - nearColor.red());
        int rg = nearColor.green() + ratio*(farColor.green() - nearColor.green());
        int rb = nearColor.blue()  + ratio*(farColor.blue()  - nearColor.blue());
        QColor arcColor(rr, rg, rb);

        painter.setPen(QPen(arcColor, 3));
        QRect arcRectRight(center.x() - r, center.y() - r, 2*r, 2*r);
        // 오른쪽 호: 0도~180도
        painter.drawArc(arcRectRight, 0, 180*16);

        painter.setPen(Qt::white);
        painter.drawText(center.x() + 10, center.y() - 10,
                         70, 20, Qt::AlignLeft,
                         QString("S2: %1mm").arg(dist2));
    }
}
