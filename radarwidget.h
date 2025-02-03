#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);

    // 센서1(좌측)와 센서2(우측)의 거리(mm)를 각각 설정
    void setSensor1Distance(int dist);
    void setSensor2Distance(int dist);
    // 임계거리 설정 (이 값 미만이면 장애물이 있다고 판단)
    void setThreshold(int threshold);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_sensor1Dist; // 좌측 센서 거리 (mm)
    int m_sensor2Dist; // 우측 센서 거리 (mm)
    int m_threshold;   // 장애물 감지 임계거리 (mm)
};

#endif // RADARWIDGET_H
