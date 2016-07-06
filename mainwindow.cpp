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

    connect(&mControllerTimer, SIGNAL(timeout()), this, SLOT(onControllerTimerTimeout()));
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
        showMsgBox(QMessageBox::Ok, QString("Error!"),  QString("Cannot resolve host name!"), NULL, QMessageBox::Critical);
    } else {
        mSocket.connectToHost(info.addresses().first(), quint16(ui->tPortNumber->text().toInt()));
        ui->statusBar->showMessage("Connecting");
        if (mSocket.waitForConnected(10000)) {
            ui->statusBar->showMessage("Connected");
        } else {
            ui->statusBar->showMessage("Disconnected");
            showMsgBox(QMessageBox::Ok, QString("Error!"),  QString("Cannot connect with host!"), NULL, QMessageBox::Critical);
            qDebug() << "Disconnected";
        }
    }
}

void MainWindow::onTcpDisconnection()
{
    ui->statusBar->showMessage("Disconnected");
    showMsgBox(QMessageBox::Ok, QString("Error!"),  QString("Connection lost!"), NULL, QMessageBox::Critical);
    ui->bConnect->setEnabled(true);
    resetGuiSettings();
    qDebug() << "Disconnected";
}

void MainWindow::onTcpReadyRead()
{
    QString message = mSocket.readAll();
    qDebug() << message;
    decodeTcpMessage(message);
}

void MainWindow::onQuit()
{
    disconnect(&mSocket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnection()));
    connect(&mSocket, SIGNAL(disconnected()),&mSocket, SLOT(deleteLater()));
}

void MainWindow::onMsgBoxAccept()
{

}

void MainWindow::onControllerTimerTimeout()
{
    if (mControllerTime) {
        ui->lControlInfo->setText("You are controlling pendulum for " +  QString::number(mControllerTime--) + " sec");
    } else {
        mControllerTimer.stop();
        ui->lControlInfo->setText("");
        ui->bControl->setEnabled(false);
        ui->bProlong->setEnabled(false);
    }
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

void MainWindow::resetGuiSettings()
{
    ui->sbCartKpNew->setEnabled(false);
    ui->sbCartKiNew->setEnabled(false);
    ui->sbCartKdNew->setEnabled(false);
    ui->sbCartNNew->setEnabled(false);
    ui->sbPendKpNew->setEnabled(false);
    ui->sbPendKiNew->setEnabled(false);
    ui->sbPendKdNew->setEnabled(false);
    ui->sbPendNNew->setEnabled(false);
    ui->sbHzNew->setEnabled(false);
    ui->hslCartPosition->setEnabled(false);
    ui->bControl->setEnabled(false);
    ui->bControl->setText("TAKE UP CONTROL");
    ui->bNewParameters->setEnabled(false);
    ui->bStart->setEnabled(false);
    ui->bProlong->setEnabled(false);
    ui->lControlInfo->setText("");
}

void MainWindow::decodeTcpMessage(QString message)
{
    QStringList tokens = message.split(" ",QString::SkipEmptyParts);

    if ((mMyAdress == NO_ADRESS) ||
            (tokens.at(1).toInt() == mMyAdress) ||
            (tokens.at(1).toInt() == BROADCAST_ADRESS)) {
        ui->statusBar->showMessage(message);

        if (tokens.at(2).compare("CONNECTED") == 0) {
            if (mMyAdress == NO_ADRESS)
                mMyAdress = tokens.at(1).toInt();
            QString informativeText;
            if (tokens.at(4).toInt() == 0) {
                ui->bControl->setEnabled(true);
                ui->lControlInfo->setText("Pendulum is not controlled");
                informativeText = "You can take control over the pendulum";
            } else {
                ui->bControl->setEnabled(false);
                ui->lControlInfo->setText("Someone is controlling the pendulum");
                informativeText = "Someone is already controlling the pendulum";
            }
            showMsgBox(QMessageBox::Ok, QString("Success!"), QString("Connected with host!"), informativeText, QMessageBox::Information);
            ui->bConnect->setEnabled(false);
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setCurrentWidget(ui->control);

        } else if (tokens.at(2).compare("PARAMS") == 0) {
            ui->sbCartKpActual->setValue(tokens.at(3).toDouble());
            ui->sbCartKpNew->setValue(tokens.at(3).toDouble());
            ui->sbCartKiActual->setValue(tokens.at(4).toDouble());
            ui->sbCartKiNew->setValue(tokens.at(4).toDouble());
            ui->sbCartKdActual->setValue(tokens.at(5).toDouble());
            ui->sbCartKdNew->setValue(tokens.at(5).toDouble());
            ui->sbCartNActual->setValue(tokens.at(6).toInt());
            ui->sbCartNNew->setValue(tokens.at(6).toInt());
            ui->sbPendKpActual->setValue(tokens.at(7).toDouble());
            ui->sbPendKpNew->setValue(tokens.at(7).toDouble());
            ui->sbPendKiActual->setValue(tokens.at(8).toDouble());
            ui->sbPendKiNew->setValue(tokens.at(8).toDouble());
            ui->sbPendKdActual->setValue(tokens.at(9).toDouble());
            ui->sbPendKdNew->setValue(tokens.at(9).toDouble());
            ui->sbPendNActual->setValue(tokens.at(10).toInt());
            ui->sbPendNNew->setValue(tokens.at(10).toInt());
            ui->sbHzActual->setValue(tokens.at(11).toInt());
            ui->sbHzNew->setValue(tokens.at(11).toInt());

        } else if (tokens.at(2).compare("CONTROL") == 0) {
            if (tokens.at(3).toInt() == ControlEnum::TakeSuccess) {
                ui->sbCartKpNew->setEnabled(true);
                ui->sbCartKiNew->setEnabled(true);
                ui->sbCartKdNew->setEnabled(true);
                ui->sbCartNNew->setEnabled(true);
                ui->sbPendKpNew->setEnabled(true);
                ui->sbPendKiNew->setEnabled(true);
                ui->sbPendKdNew->setEnabled(true);
                ui->sbPendNNew->setEnabled(true);
                ui->sbHzNew->setEnabled(true);
                ui->hslCartPosition->setEnabled(true);
                ui->bControl->setEnabled(true);
                ui->bNewParameters->setEnabled(true);
                ui->bStart->setEnabled(true);
                ui->bProlong->setEnabled(true);
                mIsController = true;
                mControllerTime = 60;
                onControllerTimerTimeout();
                mControllerTimer.start(1000);
            } else if (tokens.at(3).toInt() == ControlEnum::TakeFail) {
                showMsgBox(QMessageBox::Ok, QString("Fail!"), QString("Someone was faster..."), NULL, QMessageBox::Information);
                ui->lControlInfo->setText("Someone is controlling the pendulum");
                ui->bControl->setEnabled(false);
            } else if (tokens.at(3).toInt() == ControlEnum::Taken) {
                if (!mIsController) {
                    ui->lControlInfo->setText("Someone is controlling the pendulum");
                    ui->bControl->setEnabled(false);
                }
            } else if (tokens.at(3).toInt() == ControlEnum::Free) {
                resetGuiSettings();
                ui->bControl->setEnabled(true);
                ui->lControlInfo->setText("Pendulum is not controlled");
                if (mIsController) {
                    mIsController = false;
                    mControllerTimer.stop();
                }
            }

        }

    }
    if (message.count("ADDR") > 1) {
        decodeTcpMessage(message.mid(message.indexOf("ADDR", 5)));
    }

}

void MainWindow::prolongControllerTime()
{
    mControllerTime = 60;
    onControllerTimerTimeout();
    mControllerTimer.stop();
    mControllerTimer.start(1000);
    QString message = QString("ADDR %1 CONTROL %2 ").arg(mMyAdress).arg(ControlEnum::Prolong);
    mSocket.write(message.toLocal8Bit());
}

void MainWindow::on_bControl_clicked()
{
    if (ui->bControl->text().compare("TAKE UP CONTROL") == 0) {
        QString message = QString("ADDR %1 CONTROL %2 ").arg(mMyAdress).arg(ControlEnum::Take);
        mSocket.write(message.toLocal8Bit());
        ui->bControl->setText("LET OFF CONTROL");
    } else {
        QString message = QString("ADDR %1 CONTROL %2 ").arg(mMyAdress).arg(ControlEnum::GiveUp);
        mSocket.write(message.toLocal8Bit());
        ui->bControl->setText("TAKE UP CONTROL");
    }
}

void MainWindow::on_bStart_clicked()
{

}

void MainWindow::on_bProlong_clicked()
{
    prolongControllerTime();
}
