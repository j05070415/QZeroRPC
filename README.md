## QZeroRPC

1.基于Qt框架的元对象系统和信号槽机制实现RPC通讯

2.灵感来自于Python的zerorpc库，相对于grpc省去复杂编译过程，简单易用

3.通过在.h/.cpp文件中定义相同名称接口即可实现RPC通讯

4.目前只实现基于TCP的客户端和服务端点对点远程调用

## 用法一览

下面示例代码演示了0-5个参数客户端和服务端定义及使用。

~~~~~~~~~~cpp
//服务接口
EchoService ser;
//服务端
QRPCServer server(&ser);
if (server.bind(1122) == 0)
    qDebug() << "listen:" << 1122;
else
    qDebug() << "listen error!" << 1122;

//客户端
EchoClient client;
if (client.connectToHost("tcp://localhost:1122") != 0)
    qDebug("connect error!");
else
    qDebug() << "connected:" << "tcp://localhost:1122";

forever {
    {
        //无参数远程调用
        EclipseTimer timer;
        auto ret = client.echo0();
        auto delta = timer.eclipsed();
        a.processEvents();
        qDebug() << "resp:" << ret << delta << "us";
    }
    {
        //1个参数远程调用
        EclipseTimer timer;
        auto ret = client.echo1("hello");
        auto delta = timer.eclipsed();
        a.processEvents();
        qDebug() << "resp:" << ret << delta << "us";
    }
    {
        //2个参数远程调用
        EclipseTimer timer;
        auto ret = client.echo2("hello", 10);
        auto delta = timer.eclipsed();
        a.processEvents();
        qDebug() << "resp:" << ret << delta << "us";
    }
    {
        //3个参数远程调用
        EclipseTimer timer;
        auto ret = client.echo3("hello", 11, {"hello", "world"});
        auto delta = timer.eclipsed();
        a.processEvents();
        qDebug() << "resp:" << ret << delta << "us";
    }
    {
        //4个参数远程调用
        EclipseTimer timer;
        auto ret = client.echo4("hello", 11, 12.01234, {"hello", "world"});
        auto delta = timer.eclipsed();
        a.processEvents();
        qDebug() << "resp:" << ret << delta << "us";
    }
    {
        //5个参数远程调用
        EclipseTimer timer;
        auto ret = client.echo5("hello", 11, 12.01234, {"hello", "world"}, 1024);
        auto delta = timer.eclipsed();
        a.processEvents();
        qDebug() << "resp:" << ret << delta << "us";
    }
}
~~~~~~~~~~

