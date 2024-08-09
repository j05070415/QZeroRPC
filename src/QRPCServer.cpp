#include "QRPCServer.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QEventLoop>
#include <QRegularExpression>

#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaType>

struct QRPCServerPrivate
{
    QTcpServer server;
    int missBytes = 0;
    QByteArray missBuffer;

    QObject* service = nullptr;
    QByteArray receiveData(QTcpSocket *socket);
};

QByteArray QRPCServerPrivate::receiveData(QTcpSocket *socket)
{
    QDataStream in(socket);
    QByteArray data;
    in >> data;

    return data;
}

QRPCServer::QRPCServer(QObject* service)
    : _p(new QRPCServerPrivate)
{
    _p->service = service;

    QObject::connect(&_p->server, &QTcpServer::newConnection,
                     this, &QRPCServer::newClientConnected);

    QObject::connect(this, &QRPCServer::messageReceived,
                     this, &QRPCServer::parseMessage);
}

QRPCServer::~QRPCServer()
{
    delete _p;
}

int QRPCServer::bind(quint16 port)
{
    if (!_p->server.listen(QHostAddress::Any, port))
        return -1;

    return 0;
}

void QRPCServer::newClientConnected()
{
    QTcpSocket *socket = _p->server.nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead,
            this, &QRPCServer::readDataFromClient);
    connect(socket, &QTcpSocket::disconnected,
            this, &QRPCServer::clientDisconnected);
}

void QRPCServer::readDataFromClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (_p->missBytes > 0) {
        readMissBytes(socket);
        return;
    }

    maxRead(socket);
}

void QRPCServer::readMissBytes(QTcpSocket* socket)
{
    auto availablesize = socket->bytesAvailable();
    if (availablesize < _p->missBytes) {
        auto readed = socket->read(availablesize);
        _p->missBuffer += readed;
        _p->missBytes = _p->missBytes - readed.size();
        return;
    }
    auto readed = socket->read(_p->missBytes);
    _p->missBuffer += readed;
    availablesize -= readed.size();
    _p->missBytes -= readed.size();
    if (_p->missBytes == 0) {
        _p->missBuffer.clear();
        emit messageReceived(socket, std::move(_p->missBuffer));
    }
}

void QRPCServer::maxRead(QTcpSocket* socket)
{
    auto availablesize = socket->bytesAvailable();
    if (availablesize < 4) return;
    int size = 0;
    QDataStream in(socket);
    in >> size;
    socket->read(4);
    availablesize -= 4;
    if (availablesize < size) {
        _p->missBuffer += socket->read(availablesize);
        _p->missBytes = size - _p->missBuffer.size();

        return;
    }
    auto buff = socket->read(size);
    emit messageReceived(socket, std::move(buff));

    maxRead(socket);
}

void QRPCServer::clientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    socket->deleteLater();
}

void QRPCServer::parseMessage(QTcpSocket* socket,
                              const QByteArray &msg)
{
    QDataStream in(msg);
    while (!in.atEnd()) {
        QString methodname;
        QVariantList pars;
        int returntype;
        in >> methodname;
        in >> pars;
        in >> returntype;

        auto parcount = pars.size();
        QVariant par0 = parcount>0?pars[0]:QVariant();
        QVariant par1 = parcount>1?pars[1]:QVariant();
        QVariant par2 = parcount>2?pars[2]:QVariant();
        QVariant par3 = parcount>3?pars[3]:QVariant();
        QVariant par4 = parcount>4?pars[4]:QVariant();
        QVariant par5 = parcount>5?pars[5]:QVariant();
        QVariant par6 = parcount>6?pars[6]:QVariant();
        QVariant par7 = parcount>7?pars[7]:QVariant();
        QVariant par8 = parcount>8?pars[8]:QVariant();
        QVariant par9 = parcount>9?pars[9]:QVariant();

        auto meta = _p->service->metaObject();
        auto methodcount = meta->methodCount();
        for (int i=0; i<methodcount; ++i) {
            auto method = meta->method(i);
            if (method.name() == methodname) {
                if (returntype == QMetaType::UnknownType) {
                    qDebug() << "QMetaType::UnknownType" << methodname;
                } else if (returntype == QMetaType::Void) {
                    meta->invokeMethod(_p->service, methodname.toLatin1(),
                                       Qt::AutoConnection,
                                       QGenericArgument(par0.typeName(), par0.data()),
                                       QGenericArgument(par1.typeName(), par1.data()),
                                       QGenericArgument(par2.typeName(), par2.data()),
                                       QGenericArgument(par3.typeName(), par3.data()),
                                       QGenericArgument(par4.typeName(), par4.data()),
                                       QGenericArgument(par5.typeName(), par5.data()),
                                       QGenericArgument(par6.typeName(), par6.data()),
                                       QGenericArgument(par7.typeName(), par7.data()),
                                       QGenericArgument(par8.typeName(), par8.data()),
                                       QGenericArgument(par9.typeName(), par9.data())
                                       );
                } else {
                    auto retData = QMetaType::create(returntype);
                    auto retTypeName = QMetaType::typeName(returntype);
                    meta->invokeMethod(_p->service, methodname.toLatin1(),
                                       Qt::AutoConnection,
                                       QGenericReturnArgument(retTypeName, retData),
                                       QGenericArgument(par0.typeName(), par0.data()),
                                       QGenericArgument(par1.typeName(), par1.data()),
                                       QGenericArgument(par2.typeName(), par2.data()),
                                       QGenericArgument(par3.typeName(), par3.data()),
                                       QGenericArgument(par4.typeName(), par4.data()),
                                       QGenericArgument(par5.typeName(), par5.data()),
                                       QGenericArgument(par6.typeName(), par6.data()),
                                       QGenericArgument(par7.typeName(), par7.data()),
                                       QGenericArgument(par8.typeName(), par8.data()),
                                       QGenericArgument(par9.typeName(), par9.data())
                                       );
                    QByteArray header, response;
                    QDataStream ds(&response, QIODevice::WriteOnly);
                    ds << returntype;
                    if (!QMetaType::save(ds, returntype, retData))
                        qDebug() << "serialize failed," << retTypeName;

                    //free memory
                    QMetaType::destroy(returntype, retData);
                    QDataStream dsheader(&header, QIODevice::WriteOnly);
                    dsheader << response.size();

                    socket->write(header + response);
                }
                break;
            }
        }
    }
}
