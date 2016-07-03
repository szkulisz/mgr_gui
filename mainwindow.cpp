#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    preparePlotChart(ui->plotChart);
    preparePlotCV(ui->plotCV);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions(){
    exitAct = new QAction("&Exit", this);
    exitAct->setShortcut(QKeySequence(tr("Ctrl+X")));
    settingsAct = new QAction("&Settings", this);
    settingsAct->setShortcut(QKeySequence(tr("Ctrl+S")));
    aboutAct = new QAction("&About", this);
    aboutAct ->setShortcut(QKeySequence(tr("Ctrl+Q")));

    menuMenu = menuBar()->addMenu("&Menu");
    menuMenu->addAction(settingsAct);
    menuMenu->addSeparator();
    menuMenu->addAction(aboutAct);
    menuMenu->addAction(exitAct);
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));  //quit application
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(on_openSettings()));
}

void MainWindow::preparePlotChart(QCustomPlot *customPlot)
{
    customPlot->setLocale(locale());
        customPlot->addGraph();
        customPlot->graph(0)->setPen(QPen(Qt::blue));
        customPlot->graph(0)->setLineStyle(QCPGraph::lsStepLeft);
        customPlot->graph(0)->setName("Chart position");

        customPlot->addGraph();
        customPlot->graph(1)->setPen(QPen(Qt::red));
        customPlot->graph(1)->setName("Setpoint");
        customPlot->graph(1)->setLineStyle(QCPGraph::lsStepLeft);

        customPlot->addGraph();
        customPlot->graph(2)->setPen(QPen(Qt::green));
        customPlot->graph(2)->setName("Pendulum angle");
        customPlot->graph(2)->setLineStyle(QCPGraph::lsStepLeft);

        customPlot->xAxis->setLabel("Number of steps");
        customPlot->yAxis->setLabel("y");
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
        customPlot->legend->setVisible(true);
}

void MainWindow::preparePlotCV(QCustomPlot *customPlot)
{
    customPlot->setLocale(locale());
        customPlot->addGraph();
        customPlot->graph(0)->setPen(QPen(Qt::blue));
        customPlot->graph(0)->setLineStyle(QCPGraph::lsStepLeft);
        customPlot->graph(0)->setName("Control value");

        customPlot->xAxis->setLabel("Number of steps");
        customPlot->yAxis->setLabel("voltage [V]");
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
        customPlot->legend->setVisible(true);
}

void MainWindow::on_openSettings()
{
    this->dialog->exec();
}

void MainWindow::on_bConnect_clicked()
{
    address.setAddress(dialog->getIPAddress());
    socket->connectToHost(address, dialog->getServerPort());

}

void MainWindow::connected()
{
    ui->statusBar->showMessage(tr("Connected"));
    ui->bStart->setEnabled(true);
    ui->bChange->setEnabled(true);
    ui->hslSetpoint->setEnabled(true);
}

void MainWindow::disconnected()
{
    ui->statusBar->showMessage(tr("Disonnected"));
    ui->bStart->setEnabled(false);
    ui->bChange->setEnabled(false);
    ui->bSTOP->setEnabled(false);
    ui->hslSetpoint->setEnabled(false);

}

void MainWindow::bytesWritten(qint64 bytes)
{

}

void MainWindow::readyRead()
{
    int id=0, chart=0, pend=0;
    double CV=0;
    QString message = socket->readAll();
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);

//    qDebug() << message;
    if ( tokens.at(0).compare("ID") == 0 ) {
        id = tokens.at(1).toInt();
        if ( id%4 ==0 ) {
            chart = tokens.at(3).toInt();
            pend = tokens.at(5).toInt();
            CV = tokens.at(7).toDouble();
            ui->plotChart->graph(0)->addData(id, chart);
            ui->plotChart->graph(1)->addData(id, setpoint);
            ui->plotChart->graph(2)->addData(id, pend);
            ui->plotChart->graph(0)->removeDataBefore(id - 500);
            ui->plotChart->xAxis->setRange(id+2, 500, Qt::AlignRight);
            ui->plotChart->graph(0)->rescaleValueAxis();
            ui->plotChart->graph(1)->rescaleValueAxis(true);
            ui->plotChart->graph(2)->rescaleValueAxis(true);
            ui->plotChart->replot();

            ui->plotCV->graph(0)->addData(id, CV);
            ui->plotCV->graph(0)->removeDataBefore(id - 500);
            ui->plotCV->xAxis->setRange(id+2, 500, Qt::AlignRight);
            ui->plotCV->graph(0)->rescaleValueAxis();
            ui->plotCV->replot();
        }
    }
}

void MainWindow::on_bStart_clicked()
{
    emit ui->hslSetpoint->sliderReleased();
    socket->write("START");
    socket->flush();
    ui->bStart->setEnabled(false);
    ui->bChange->setEnabled(false);
    ui->bSTOP->setEnabled(true);
}

void MainWindow::on_bSTOP_clicked()
{
    socket->write("STOP");
    socket->flush();
    ui->bStart->setEnabled(true);
    ui->bChange->setEnabled(true);
    ui->bSTOP->setEnabled(false);
}

void MainWindow::closing()
{
    connect(socket, SIGNAL(disconnected()),
            socket, SLOT(deleteLater()));
    socket->disconnectFromHost();

}

void MainWindow::on_bChange_clicked()
{
    double K = ui->dspK->value();
    double Ti = ui->dspTi->value();
    double Td = ui->dspTd->value();
    QString message = QString("PARAMS K %1 Ti %2 Td %3").arg(K).arg(Ti).arg(Td);
    qDebug() << message;
    socket->write(message.toLocal8Bit());
}

void MainWindow::on_hslSetpoint_valueChanged(int value)
{

}

void MainWindow::on_hslSetpoint_sliderReleased()
{
    setpoint = ui->hslSetpoint->value();
    QString message = QString("SETPOINT %1").arg(setpoint);
    qDebug() << message;
    socket->write(message.toLocal8Bit());
    socket->flush();
}

