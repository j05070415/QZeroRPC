#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H

#include <QObject>

class EchoService : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QString echo0();
    Q_INVOKABLE QString echo1(const QString& txt);
    Q_INVOKABLE QString echo2(const QString& txt, int value0);
    Q_INVOKABLE QString echo3(const QString& txt, int value0, const QStringList& value1);
    Q_INVOKABLE QString echo4(const QString& txt, int value0,
                              double value1, const QStringList& value2);
    Q_INVOKABLE QString echo5(const QString& txt, int value0,
                              double value1, const QStringList& value2, int value3);
};

#endif // ECHOSERVICE_H
