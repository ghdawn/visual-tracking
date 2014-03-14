#include <QApplication>
#include <QtGui>
#include <QMutex>
#include <cstdio>
#include "trackcore.h"
#include "processimage.h"
#include "camerathread.h"
#include "joystickthread.h"
#include "trackthread.h"
using namespace itr_tracker;
using namespace std;
int main(int argc, char *argv[])
{
    itr_math::MathObjStandInit();
    printf("Hello\n");
    QApplication a(argc, argv);
    TrackCore core;
    QPainter paint;
    QImage Mimg;
    core.Init(320,240);
    QMutex mutexCurrent;
    QMutex mutexPost;

    CameraThread camera;
    camera.Init(&core);
    camera.mutexCurrent=&mutexCurrent;
    camera.mutexPost=&mutexPost;

    TrackThread track;
    track.Init(&core);
    track.mutexCurrent=&mutexCurrent;
    while(1)
    {

        if(core.NewPostImg==true)
            Mimg=QImage(core.postImg, 320, 240, QImage::Format_RGB32);
            paint.drawImage(QPoint(0, 0), Mimg);
            core.NewPostImg=false;

    }

    return a.exec();
}



/*#include <QPainter>
#include "serialport.h"
#include "processimage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RectangleF rect;
    RectangleF(10,10,100,100);
    U8 outputimg[320*240];
    string Info;
    Info = "Succeed";
    ProcessImage process;
    process.Init(320,240);
    QImage Img = QImage("a.png");
    unsigned char * p_bits=Img.bits();
    process.Process(p_bits,rect,Info,outputimg);
    QImage img = QImage(outputimg, 320, 240, QImage::Format_RGB32);
    QPainter paint;
    paint.drawImage(QPoint(0, 0), img);
    return a.exec();
}

include <QtGui/QApplication>
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
}*/

/*#include <QtGui/QApplication>
#include "mainwindow.h"
#include "trackcore.h"
using namespace itr_tracker;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMutex mutex;
   // Thread thread;
    TrackCore core;
   // thread.start();
    CameraThread CameraThread("Camera");
    CameraThread.Init(&core);
   // CameraThread.start();
    TrackThread TrackThread("Track");
    TrackThread.Init(&core);
    JoyStickThread JoyStickThread("JoyStick");
    JoyStickThread.Init(&core);
    JoyStickThread.start();
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
