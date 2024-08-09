#ifndef QRPCSERVER_H
#define QRPCSERVER_H

#include <QObject>

class QTcpSocket;
struct QRPCServerPrivate;
class QRPCServer : public QObject
{
    Q_OBJECT
public:
    QRPCServer(QObject* service);
    ~QRPCServer();

    int bind(quint16 ipport);

private slots:
    void newClientConnected();
    void readDataFromClient();
    void maxRead(QTcpSocket* socket);
    void clientDisconnected();
    void parseMessage(QTcpSocket* socket,
                      const QByteArray& msg);

signals:
    void messageReceived(QTcpSocket*,
                         const QByteArray&);

private:
    void readMissBytes(QTcpSocket* socket);

private:
    QRPCServerPrivate* _p;
};

#endif // QRPCSERVER_H
