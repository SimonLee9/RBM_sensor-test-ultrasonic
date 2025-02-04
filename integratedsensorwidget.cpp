#include "integratedsensorwidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QApplication>  // QApplication::beep()

IntegratedSensorWidget::IntegratedSensorWidget(QWidget *parent)
    : QWidget(parent),
      m_novaTop(0), m_novaRight(0), m_novaBottom(0), m_novaLeft(0),
      m_dfLeftFront1(0), m_dfLeftFront2(0), m_dfRightFront1(0), m_dfRightFront2(0),
      m_threshold(300)  // 300 mm (30 cm) 기준
{
    setMinimumSize(600, 600);
}

void IntegratedSensorWidget::setThreshold(int threshold)
{
    if(m_threshold != threshold) {
        m_threshold = threshold;
        update();
    }
}

// Nova‑A4 센서 setter들
void IntegratedSensorWidget::setNovaTop(int value)
{
    m_novaTop = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::setNovaRight(int value)
{
    m_novaRight = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::setNovaBottom(int value)
{
    m_novaBottom = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::setNovaLeft(int value)
{
    m_novaLeft = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

// DFROBOT 센서 setter들
void IntegratedSensorWidget::setDFLeftFront1(int value)
{
    m_dfLeftFront1 = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::setDFLeftFront2(int value)
{
    m_dfLeftFront2 = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::setDFRightFront1(int value)
{
    m_dfRightFront1 = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::setDFRightFront2(int value)
{
    m_dfRightFront2 = value;
    if(value > 0 && value < m_threshold)
        QApplication::beep();
    update();
}

void IntegratedSensorWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int w = width(), h = height();

    // 차량(사각형) 표시: 중앙 영역
    QRect vehicleRect(w * 0.3, h * 0.3, w * 0.4, h * 0.4);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRect(vehicleRect);

    // Nova‑A4 센서 영역 (각 변)
    // 영역의 배경색은 해당 측정값이 300 mm 미만이면 붉게, 아니면 초록색으로 표시

    // Top 영역: 차량 위쪽 중앙
    QRect novaTopRect(vehicleRect.left(), vehicleRect.top() - 40, vehicleRect.width(), 30);
    QColor novaTopColor = (m_novaTop > 0 && m_novaTop < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(novaTopRect, novaTopColor);
    painter.setPen(Qt::black);
    painter.drawText(novaTopRect, Qt::AlignCenter, QString("%1 mm").arg(m_novaTop));

    // Right 영역: 차량 오른쪽 중앙
    QRect novaRightRect(vehicleRect.right() + 10, vehicleRect.top(), 100, vehicleRect.height());
    QColor novaRightColor = (m_novaRight > 0 && m_novaRight < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(novaRightRect, novaRightColor);
    painter.setPen(Qt::black);
    painter.drawText(novaRightRect, Qt::AlignCenter, QString("%1 mm").arg(m_novaRight));

    // Bottom 영역: 차량 아래쪽 중앙
    QRect novaBottomRect(vehicleRect.left(), vehicleRect.bottom() + 10, vehicleRect.width(), 30);
    QColor novaBottomColor = (m_novaBottom > 0 && m_novaBottom < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(novaBottomRect, novaBottomColor);
    painter.setPen(Qt::black);
    painter.drawText(novaBottomRect, Qt::AlignCenter, QString("%1 mm").arg(m_novaBottom));

    // Left 영역: 차량 왼쪽 중앙
    QRect novaLeftRect(vehicleRect.left() - 110, vehicleRect.top(), 100, vehicleRect.height());
    QColor novaLeftColor = (m_novaLeft > 0 && m_novaLeft < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(novaLeftRect, novaLeftColor);
    painter.setPen(Qt::black);
    painter.drawText(novaLeftRect, Qt::AlignCenter, QString("%1 mm").arg(m_novaLeft));

    // DFROBOT 센서 영역 – 전면 모서리에 2개씩
    // 왼쪽 전면 센서: 차량의 왼쪽 위 모서리 근처
    QRect dfLeft1Rect(vehicleRect.left() - 100, vehicleRect.top() - 30, 90, 25);
    QColor dfLeft1Color = (m_dfLeftFront1 > 0 && m_dfLeftFront1 < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(dfLeft1Rect, dfLeft1Color);
    painter.setPen(Qt::black);
    painter.drawText(dfLeft1Rect, Qt::AlignCenter, QString("DF LF1: %1 mm").arg(m_dfLeftFront1));

    QRect dfLeft2Rect(vehicleRect.left(), vehicleRect.top() - 30, 90, 25);
    QColor dfLeft2Color = (m_dfLeftFront2 > 0 && m_dfLeftFront2 < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(dfLeft2Rect, dfLeft2Color);
    painter.setPen(Qt::black);
    painter.drawText(dfLeft2Rect, Qt::AlignCenter, QString("DF LF2: %1 mm").arg(m_dfLeftFront2));

    // 오른쪽 전면 센서: 차량의 오른쪽 위 모서리 근처
    QRect dfRight1Rect(vehicleRect.right(), vehicleRect.top() - 30, 90, 25);
    QColor dfRight1Color = (m_dfRightFront1 > 0 && m_dfRightFront1 < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(dfRight1Rect, dfRight1Color);
    painter.setPen(Qt::black);
    painter.drawText(dfRight1Rect, Qt::AlignCenter, QString("RF1: %1 mm").arg(m_dfRightFront1));

    QRect dfRight2Rect(vehicleRect.right() - 90, vehicleRect.top() - 30, 90, 25);
    QColor dfRight2Color = (m_dfRightFront2 > 0 && m_dfRightFront2 < m_threshold) ? Qt::red : Qt::green;
    painter.fillRect(dfRight2Rect, dfRight2Color);
    painter.setPen(Qt::black);
    painter.drawText(dfRight2Rect, Qt::AlignCenter, QString("RF2: %1 mm").arg(m_dfRightFront2));
}
