#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readData();             // 시리얼 데이터 수신 처리 슬롯

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    // 수신 패킷 임시 저장 버퍼
    QByteArray m_buffer;
};

#endif // MAINWINDOW_H
