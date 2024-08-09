#ifndef QRPCCLIENT_P_H
#define QRPCCLIENT_P_H

#include <QRPCClient.h>

#include <QEventLoop>
#include <QDataStream>
#include <QTimer>

template<typename T>
T QRPCClient::sendSync(const QString& funcname,
                       const QVariantList& pars)
{
    //序列化函数
    int returnType = QMetaType::type(typeid (T).name());
    if (returnType == QMetaType::UnknownType) {
        qDebug() << "-->Error, UnknownType" << typeid (T).name();
        return {};
    }
    QByteArray serialdata;
    QDataStream ds(&serialdata, QIODevice::WriteOnly);
    ds << funcname;
    ds << pars;
    ds << returnType;

    T response;
    QEventLoop loop;
    connect(this, &QRPCClient::response, this, [&](const QByteArray& data){
        QDataStream respds(data);
        int type = 0;
        respds >> type;
        if (!QMetaType::load(respds, type, &response))
            qDebug() << "unserial failed," << QMetaType::typeName(type);

        loop.exit(0);
    });
    QTimer timer;
    connect(&timer, &QTimer::timeout, [&loop](){
        loop.exit(-1);
    });

    //发送数据
    emit startSend(serialdata);

    //超时3s
    timer.start(3000);
    if (loop.exec() != 0)
        qDebug() << "time out, no response!";

    disconnect(this, &QRPCClient::response, 0, 0);

    //返回响应数据
    return response;
}

#endif // QRPCCLIENT_P_H
