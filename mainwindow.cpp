#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>
#include "qcustomplot-source/qcustomplot.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Disconnected");
    ui->tabWidget->setTabEnabled(1, false);
    preparePlot(ui->plotCart);
    preparePlot(ui->plotCV);
    preparePlot(ui->plotPendulum);

    connect(&socket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnection()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bConnect_clicked()
{
    QHostInfo info = QHostInfo::fromName(ui->tHostName->text());
    qDebug() << info.hostName();
    qDebug() << info.addresses();
    if (info.addresses().isEmpty()) {
        QMessageBox msgBox(QMessageBox::Critical, QString("Error!"), QString("Cannot resolve host name!"), QMessageBox::Ok);
        msgBox.exec();
    } else {
        socket.connectToHost(info.addresses().first(), quint16(ui->tPortNumber->text().toInt()));
        ui->statusBar->showMessage("Connecting");
        if (socket.waitForConnected(10000)) {
            ui->statusBar->showMessage("Connected");
            QMessageBox msgBox(QMessageBox::Information, QString("Success!"), QString("Connected with host!"), QMessageBox::Ok);
            msgBox.exec();
            ui->bConnect->setEnabled(false);
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setCurrentWidget(ui->control);
        } else {
            ui->statusBar->showMessage("Disconnected");
            QMessageBox msgBox(QMessageBox::Critical, QString("Error!"), QString("Cannot connect with host!"), QMessageBox::Ok);
            msgBox.exec();
            qDebug() << "Disconnected";
        }
    }
}

void MainWindow::onTcpDisconnection()
{
    ui->statusBar->showMessage("Disconnected");
    QMessageBox msgBox(QMessageBox::Critical, QString("Error!"), QString("Connection lost!"), QMessageBox::Ok);
    msgBox.exec();
    ui->bConnect->setEnabled(true);
    qDebug() << "Disconnected";
}

void MainWindow::onQuit()
{
    disconnect(&socket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnection()));
    connect(&socket, SIGNAL(disconnected()),&socket, SLOT(deleteLater()));
}

void MainWindow::preparePlot(QCustomPlot *plot)
{
    plot->setLocale(locale());
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->graph(0)->setLineStyle(QCPGraph::lsStepLeft);

    if (plot->objectName() == QString("plotCart")) {
        plot->graph(0)->setName("Cart position");
        plot->addGraph();
        plot->graph(1)->setPen(QPen(Qt::red));
        plot->graph(1)->setName("Setpoint");
        plot->graph(1)->setLineStyle(QCPGraph::lsStepLeft);
        plot->yAxis->setLabel("Cart position, m");
    } else if (plot->objectName() == QString("plotPendulum")) {
        plot->graph(0)->setName("Pendulum angle");
        plot->yAxis->setLabel("Pendulum angle, rad");
    } else {
        plot->graph(0)->setName("Control value");
        plot->yAxis->setLabel("Control value, V");
        plot->xAxis->setLabel("Time, s");
    }
    plot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
}
