#ifndef INTEGRATEDSENSORWIDGET_H
#define INTEGRATEDSENSORWIDGET_H

#include <QWidget>

class IntegratedSensorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IntegratedSensorWidget(QWidget *parent = nullptr);

    // Nova‑A4 센서용 (상, 우, 하, 좌)
    void setNovaTop(int value);
    void setNovaRight(int value);
    void setNovaBottom(int value);
    void setNovaLeft(int value);

    // DFROBOT 센서용 (전면 좌측 2개, 전면 우측 2개)
    void setDFLeftFront1(int value);
    void setDFLeftFront2(int value);
    void setDFRightFront1(int value);
    void setDFRightFront2(int value);

    // 임계치(단위 mm)를 설정 (기본 300 mm)
    void setThreshold(int threshold);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    // Nova‑A4 센서 측정값
    int m_novaTop;
    int m_novaRight;
    int m_novaBottom;
    int m_novaLeft;
    // DFROBOT 센서 측정값
    int m_dfLeftFront1;
    int m_dfLeftFront2;
    int m_dfRightFront1;
    int m_dfRightFront2;
    // 임계치 (300 mm 미만이면 경고)
    int m_threshold;
};

#endif // INTEGRATEDSENSORWIDGET_H
