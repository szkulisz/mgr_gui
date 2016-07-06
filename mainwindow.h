#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QThread>
#include <QMessageBox>

class QCustomPlot;

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

    void on_bControl_clicked();

private:
    const int NO_ADRESS = -20;
    const int BROADCAST_ADRESS = -10;
    const int TAKE_CONTROL_SUCCESS = 3;
    const int TAKE_CONTROL_FAIL = 4;

    Ui::MainWindow *ui;
    QTcpSocket mSocket;
    int mMyAdress = NO_ADRESS;    

    void preparePlot(QCustomPlot *plot);
    void showMsgBox(QMessageBox::StandardButton button, QString title, QString text, QString info, QMessageBox::Icon icon);
    void resetGuiSettings();
    void decodeTcpMessage(QString message);
};

#endif // MAINWINDOW_H
