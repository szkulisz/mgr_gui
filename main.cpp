#include "mainwindow.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setFixedSize(w.width(), w.height());
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(onQuit()));

    return a.exec();
}
