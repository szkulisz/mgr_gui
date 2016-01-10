#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
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

void MainWindow::on_openSettings()
{
    this->dialog->exec();
}
