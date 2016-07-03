#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void onQuit();

private:
    Ui::MainWindow *ui;
    QTcpSocket socket;

    void preparePlot(QCustomPlot *plot);
};

#endif // MAINWINDOW_H
