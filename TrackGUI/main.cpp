#include <QApplication>
#include "serialport.h"
#include "processimage.h"

int main()
{

}
/*#include <QtGui/QApplication>
#include <QTextCodec>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb2312"));
    Widget w;
    w.show();
    return a.exec();
}
#include "udpserver.h"
#include "udpclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UdpServer *server = new UdpServer;
    UdpClient *client = new UdpClient;
    server->show();
    client->show();
    return app.exec();
}

#include <QtGui/QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMutex mutex;
    //Thread thread;

    //thread.start();
    Thread thread1("Thread1");
    thread1.mutex=&mutex;
    thread1.start();
    Thread thread2("Thread2");
    thread2.mutex=&mutex;
    thread2.start();

    return app.exec();
}
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc,char *argv[]){
   QApplication a(argc,argv);
   MainWindow window;
   window.show();
   return a.exec();
}*/
// 用来测试串口的主程序
/*#include<QApplication>
#include"mainwindow.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    MainWindow mw;
    mw.show();
    return app.exec();
}*/

//用来测试图像处理的主程序
/*#include <QtGui>
#include "processimage.h"

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    ProcessImage process;
    int width;
    int height;
    width = 320;
    height =240;
    process.resize(width,height);
    process.show();

    return app.exec();
}*/
