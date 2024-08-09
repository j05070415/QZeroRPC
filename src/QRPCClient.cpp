#include "QRPCClient.h"

#include <QTcpSocket>
#include <QThread>
#include <QRegularExpression>

struct QRPCClientPrivate
{
    QTcpSocket sock;
    QThread sendthread;
    QByteArray missBuffer;
    int missBytes = 0;

    QString ip;
    quint16 port;
};

QRPCClient::QRPCClient(QObject *parent) : QObject(parent),
    _p(new QRPCClientPrivate)
{
    this->moveToThread(&_p->sendthread);
    _p->sock.moveToThread(&_p->sendthread);
    _p->sendthread.start();

    connect(this, &QRPCClient::startSend,
            this, &QRPCClient::onStartSend);
    connect(&_p->sock, &QTcpSocket::readyRead,
            this, &QRPCClient::onMessageReceive);
    connect(&_p->sock, &QTcpSocket::disconnected,
            this, [this](){
        _p->missBytes = 0;
        _p->missBuffer = {};
    });
    connect(&_p->sock, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(const QAbstractSocket::SocketError&)));
}

QRPCClient::~QRPCClient()
{
    delete _p;
}

//tcp://localhost:1111
int QRPCClient::connectToHost(const QString &ipport)
{
    QRegularExpression regex("^(.+)://(.+):(.+)$");

    QRegularExpressionMatch match = regex.match(ipport);
    if (match.hasMatch()) {
        QString protocol = match.captured(1);
        QString ip = match.captured(2);
        auto port = match.captured(3).toUShort();

        QEventLoop loop;
        _p->sock.connectToHost(ip, port);
        connect(&_p->sock, &QTcpSocket::connected,
                [&loop](){
            loop.quit();
        });
        loop.exec();
        _p->ip = ip;
        _p->port = port;
        return 0;
    }

    return -1;
}

void QRPCClient::onStartSend(const QByteArray &data)
{
    if (!_p->sock.isValid()) {
        _p->sock.connectToHost(_p->ip, _p->port);
    }
    QDataStream ds(&_p->sock);
    ds << data.size();
    ds << data;
}

void QRPCClient::onMessageReceive()
{
    if (_p->missBytes > 0) {
        readMissBytes();
        return;
    }

    maxRead();
}

void QRPCClient::readMissBytes()
{
    auto availablesize = _p->sock.bytesAvailable();
    if (availablesize < _p->missBytes) {
        auto readed = _p->sock.read(availablesize);
        _p->missBuffer += readed;
        _p->missBytes = _p->missBytes - readed.size();
        return;
    }
    auto readed = _p->sock.read(_p->missBytes);
    _p->missBuffer += readed;
    availablesize -= readed.size();
    _p->missBytes -= readed.size();
    if (_p->missBytes == 0) {
        emit response(std::move(_p->missBuffer));
    }
}

void QRPCClient::maxRead()
{
    auto availablesize = _p->sock.bytesAvailable();
    if (availablesize < 4) return;
    int size = 0;
    QDataStream sizeds(&_p->sock);
    sizeds >> size;
    availablesize -= 4;
    if (availablesize < size) {
        auto readed = _p->sock.read(availablesize);
        _p->missBuffer += readed;
        _p->missBytes = size - readed.size();

        return;
    }
    auto readed = _p->sock.read(size);
    if (readed.size() != size) qDebug() << "QRPCClient::maxRead() error";
    emit response(readed);

    maxRead();
}

void QRPCClient::onError(const QAbstractSocket::SocketError& err)
{
    qDebug() << err;
    _p->missBytes = 0;
    _p->missBuffer = {};
}
