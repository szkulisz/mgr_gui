#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serversettingsdialog.h"

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

    void createActions();

protected:

private slots:
//    void about();
    void on_openSettings();
};

#endif // MAINWINDOW_H
