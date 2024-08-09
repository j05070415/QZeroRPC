#include "echoservice.h"

#include <QDebug>
#include <QDataStream>
#include <QVector>
#include <QString>

QString EchoService::echo0()
{
    return "EchoServer::echo0()";
}

QString EchoService::echo1(const QString &txt)
{
    return "EchoServer::echo1," + txt;
}

QString EchoService::echo2(const QString &txt, int value0)
{
    return QString("EchoServer::echo2,%1,%2").arg(txt).arg(value0);
}

QString EchoService::echo3(const QString &txt, int value0, const QStringList& value1)
{
    return QString("EchoServer::echo3,%1,%2,%3").arg(txt).arg(value0).arg(value1.join(":"));
}

QString EchoService::echo4(const QString &txt, int value0, double value1, const QStringList &value2)
{
    return QString("EchoServer::echoe4,%1,%2,%3,%4")
            .arg(txt).arg(value0).arg(value1).arg(value2.join(":"));
}

QString EchoService::echo5(const QString &txt, int value0, double value1, const QStringList &value2, int value3)
{
    return QString("EchoServer::echoe5,%1,%2,%3,%4,%5")
            .arg(txt).arg(value0).arg(value1).arg(value2.join(":"))
            .arg(value3);
}
