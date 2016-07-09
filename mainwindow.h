#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QThread>
#include <QMessageBox>
#include <QTimer>

class QCustomPlot;

enum ControlEnum {
    Taken,
    Free,
    Take,
    TakeSuccess,
    TakeFail,
    GiveUp,
    Prolong
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_bConnect_clicked();
    void onTcpDisconnection();
    void onTcpReadyRead();
    void onQuit();
    void onMsgBoxAccept();
    void onControllerTimerTimeout();

    void on_bControl_clicked();

    void on_bStart_clicked();

    void on_bProlong_clicked();

    void on_bNewParameters_clicked();

    void on_hslCartPosition_sliderReleased();

    void on_pushButton_clicked();

private:
    const int NO_ADRESS = -20;
    const int BROADCAST_ADRESS = -10;

    Ui::MainWindow *ui;
    QTcpSocket mSocket;
    int mMyAdress = NO_ADRESS;    
    QTimer mControllerTimer;
    int mControllerTime;
    bool mIsController = false;
    bool mIsStarted = false;

    void preparePlot(QCustomPlot *plot);
    void showMsgBox(QMessageBox::StandardButton button, QString title, QString text, QString info, QMessageBox::Icon icon);
    void initGuiSettings();
    void resetGui();
    void decodeTcpMessage(QString message);
    void prolongControllerTime();
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
