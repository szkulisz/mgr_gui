#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Disconnected");
    QTimer::singleShot(0, ui->tHostName, SLOT(setFocus()));


//    connect(QApplication, SIGNAL(aboutToQuit()), this, SLOT(onQuit()));
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
    //disconnect(&socket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnection()));
connect(&socket, SIGNAL(disconnected()),
            &socket, SLOT(deleteLater()));
    socket.disconnectFromHost();
}
