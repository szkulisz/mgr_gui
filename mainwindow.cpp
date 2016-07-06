#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <QHostAddress>
#include <QTimer>
#include <unistd.h>
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

    connect(&mSocket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnection()));
    connect(&mSocket, SIGNAL(readyRead()), this, SLOT(onTcpReadyRead()));

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
        mSocket.connectToHost(info.addresses().first(), quint16(ui->tPortNumber->text().toInt()));
        ui->statusBar->showMessage("Connecting");
        if (mSocket.waitForConnected(10000)) {
            ui->statusBar->showMessage("Connected");
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

void MainWindow::onTcpReadyRead()
{
    QString message = mSocket.readAll();
    qDebug() << message;
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);

    if ((mMyAdress == NO_ADRESS) ||
            (tokens.at(0).toInt() == mMyAdress) ||
            (tokens.at(0).toInt() == BROADCAST_ADRESS)) {
        ui->statusBar->showMessage(message);

        if (tokens.at(1).compare("CONNECTED") == 0) {
            if (mMyAdress == NO_ADRESS)
                mMyAdress = tokens.at(0).toInt();
            QString informativeText;
            if (tokens.at(3).toInt() == 0) {
                ui->bControl->setEnabled(true);
                informativeText = "You can take control over the pendulum";
            } else {
                ui->bControl->setEnabled(false);
                informativeText = "Someone is already controlling the pendulum";
            }
            showMsgBox(QMessageBox::Ok, QString("Success!"), QString("Connected with host!"), informativeText, QMessageBox::Information);
            ui->bConnect->setEnabled(false);
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setCurrentWidget(ui->control);
        } else if (tokens.at(1).compare("PARAMS") == 0) {
            ui->sbCartKpActual->setValue(tokens.at(2).toDouble());
            ui->sbCartKpNew->setValue(tokens.at(2).toDouble());
            ui->sbCartKiActual->setValue(tokens.at(3).toDouble());
            ui->sbCartKiNew->setValue(tokens.at(3).toDouble());
            ui->sbCartKdActual->setValue(tokens.at(4).toDouble());
            ui->sbCartKdNew->setValue(tokens.at(4).toDouble());
            ui->sbCartNActual->setValue(tokens.at(5).toInt());
            ui->sbCartNNew->setValue(tokens.at(5).toInt());
            ui->sbPendKpActual->setValue(tokens.at(6).toDouble());
            ui->sbPendKpNew->setValue(tokens.at(6).toDouble());
            ui->sbPendKiActual->setValue(tokens.at(7).toDouble());
            ui->sbPendKiNew->setValue(tokens.at(7).toDouble());
            ui->sbPendKdActual->setValue(tokens.at(8).toDouble());
            ui->sbPendKdNew->setValue(tokens.at(8).toDouble());
            ui->sbPendNActual->setValue(tokens.at(9).toInt());
            ui->sbPendNNew->setValue(tokens.at(9).toInt());
            ui->sbHzActual->setValue(tokens.at(10).toInt());
            ui->sbHzNew->setValue(tokens.at(10).toInt());

        }

    }
}

void MainWindow::onQuit()
{
    disconnect(&mSocket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnection()));
    connect(&mSocket, SIGNAL(disconnected()),&mSocket, SLOT(deleteLater()));
}

void MainWindow::onMsgBoxAccept()
{

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

void MainWindow::showMsgBox(QMessageBox::StandardButton button, QString title, QString text, QString info, QMessageBox::Icon icon)
{
    QMessageBox* msgBox = new QMessageBox( this );
    msgBox->setAttribute( Qt::WA_DeleteOnClose ); //makes sure the msgbox is deleted automatically when closed
    msgBox->setStandardButtons( button );
    msgBox->setWindowTitle( title );
    msgBox->setText( text );
    msgBox->setIcon( icon );
    msgBox->setInformativeText( info );
    msgBox->open(this,SLOT(onMsgBoxAccept()));
}

void MainWindow::on_bControl_clicked()
{
    mSocket.write("ELO");
}
