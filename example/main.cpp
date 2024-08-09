#include <QApplication>

#include "EclipseTimer.hpp"

#include "echoservice.h"
#include "echoclient.h"

#include "QRPCClient.h"
#include "QRPCServer.h"

#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EchoService ser;
    QRPCServer server(&ser);
    if (server.bind(1122) == 0)
        qDebug() << "listen:" << 1122;
    else
        qDebug() << "listen error!" << 1122;

    EchoClient client;
    if (client.connectToHost("tcp://localhost:1122") != 0)
        qDebug("connect error!");
    else
        qDebug() << "connected:" << "tcp://localhost:1122";

    forever {
        {
            EclipseTimer timer;
            auto ret = client.echo0();
            auto delta = timer.eclipsed();
            a.processEvents();
            qDebug() << "resp:" << ret << delta << "us";
        }
        {
            EclipseTimer timer;
            auto ret = client.echo1("hello");
            auto delta = timer.eclipsed();
            a.processEvents();
            qDebug() << "resp:" << ret << delta << "us";
        }
        {
            EclipseTimer timer;
            auto ret = client.echo2("hello", 10);
            auto delta = timer.eclipsed();
            a.processEvents();
            qDebug() << "resp:" << ret << delta << "us";
        }
        {
            EclipseTimer timer;
            auto ret = client.echo3("hello", 11, {"hello", "world"});
            auto delta = timer.eclipsed();
            a.processEvents();
            qDebug() << "resp:" << ret << delta << "us";
        }
        {
            EclipseTimer timer;
            auto ret = client.echo4("hello", 11, 12.01234, {"hello", "world"});
            auto delta = timer.eclipsed();
            a.processEvents();
            qDebug() << "resp:" << ret << delta << "us";
        }
        {
            EclipseTimer timer;
            auto ret = client.echo5("hello", 11, 12.01234, {"hello", "world"}, 1024);
            auto delta = timer.eclipsed();
            a.processEvents();
            qDebug() << "resp:" << ret << delta << "us";
        }
    }

    return a.exec();
}
