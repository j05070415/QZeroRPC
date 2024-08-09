#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include "QRPCClient.h"

#include <QObject>
#include <QVector>
#include <QString>

class EchoClient : public QRPCClient
{
    Q_OBJECT
public:
    METHOD0(echo0, QString);
    METHOD1(echo1, QString, QString);
    METHOD2(echo2, QString, QString, int);
    METHOD3(echo3, QString, QString, int, QStringList);
    METHOD4(echo4, QString, QString, int, double, QStringList);
    METHOD5(echo5, QString, QString, int, double, QStringList, int)
};

#endif // ECHOCLIENT_H
