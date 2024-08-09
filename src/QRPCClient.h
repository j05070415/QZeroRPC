#ifndef QRPCCLIENT_H
#define QRPCCLIENT_H

#include "QZeroRpcGlobal.h"

#include <QObject>
#include <QVariantList>
#include <QAbstractSocket>

struct QRPCClientPrivate;
class QRPCClient : public QObject
{
    Q_OBJECT
public:
    explicit QRPCClient(QObject *parent = nullptr);
    ~QRPCClient();

    int connectToHost(const QString& ipport);

signals:
    void startSend(const QByteArray&);
    void response(const QByteArray&);

protected:
    template<typename T>
    T sendSync(const QString& funcname,
               const QVariantList& pars);

private slots:
    void onStartSend(const QByteArray& data);
    void onMessageReceive();
    void maxRead();

private slots:
    void onError(const QAbstractSocket::SocketError& err);

private:
    void readMissBytes();

private:
    QRPCClientPrivate* _p;
};

#include "QRPCClient_p.h"

#endif // QRPCCLIENT_H
