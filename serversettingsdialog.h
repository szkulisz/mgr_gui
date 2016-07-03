#ifndef SERVERSETTINGSDIALOG_H
#define SERVERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ServerSettingsDialog;
}

class ServerSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerSettingsDialog(QWidget *parent = 0);
    ~ServerSettingsDialog();
    quint32 virtual getIPAddress();    // return the IP address as a 32-bit int
    quint16 virtual getServerPort() { return serverPortNumber; } // the port number

    void setServerPortNumber(int value);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ServerSettingsDialog *ui;
    quint16 serverPortNumber;              // port number (default 5555)
    int address[4];                    // IP address (default 192.168.7.2)
};

#endif // SERVERSETTINGSDIALOG_H
