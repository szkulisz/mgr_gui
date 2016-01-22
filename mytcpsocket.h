#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);
    void doConnect();

private:
    QTcpSocket *socket;
    QHostAddress *adress;
signals:

public slots:
//    void connected();
//    void disconnected();
//    void bytesWritten(qint64 bytes);
//    void readyRead();
};

#endif // MYTCPSOCKET_H
