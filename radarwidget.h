#ifndef RADARWIDGET_H
#define RADARWIDGET_H

#include <QWidget>

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);

    // 센서1, 센서2의 거리(mm)를 각각 설정
    void setSensor1Distance(int dist);
    void setSensor2Distance(int dist);

    // 최대 표시 거리(기본 3m) 설정
    void setMaxDistance(int maxDist);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_sensor1Dist;
    int m_sensor2Dist;
    int m_maxDist;
};

#endif // RADARWIDGET_H
