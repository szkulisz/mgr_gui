#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include "mytcpsocket.h"
#include "serversettingsdialog.h"
#include "qcustomplot-source/qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu *menuMenu;
    QAction *settingsAct;
    QAction *aboutAct;
    QAction *exitAct;
    ServerSettingsDialog *dialog = new ServerSettingsDialog();
    MyTcpSocket mySocket;
    QTcpSocket *socket;
    QHostAddress address;
    int setpoint = 0;


    void createActions();
    void preparePlotChart(QCustomPlot* customPlot);
    void preparePlotCV(QCustomPlot* customPlot);

protected:

private slots:
//    void about();
    void on_openSettings();
    void on_bConnect_clicked();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void on_bStart_clicked();
    void on_bSTOP_clicked();
    void closing();
    void on_bChange_clicked();
    void on_hslSetpoint_valueChanged(int value);
    void on_hslSetpoint_sliderReleased();
};

#endif // MAINWINDOW_H
