#include "mainwindow.h"
#include "itrbase.h"
#include "trackcore.h"
#include "trackthread.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>
#include "processimage.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(320,240);
    w.show();
    /*itr_math::MathObjStandInit();
    printf("Hello\n");
    TrackCore core;
    QPainter paint;
    QImage Mimg;
    core.Init(320,240);
    S32 i;
    ProcessImage process;
    process.Init(320,240);

     QMutex mutexCurrent;
        QMutex mutexPost;

        CameraThread camera;
        camera.Init(&core);
        camera.mutexCurrent=&mutexCurrent;
        camera.mutexPost=&mutexPost;

        TrackThread track;

    for(i=0; i < core.Width*core.Height; i++)
    {
        core.postImg[i] = 128;
    }
    track.mutexCurrent=&mutexCurrent;
    Mimg=QImage(core.postImg, 320, 240, QImage::Format_RGB32);
    paint.drawImage(QPoint(0, 0), Mimg);
     while(1)
        {
            if(core.NewPostImg==true)
            {
                Mimg=QImage(core.postImg, 320, 240, QImage::Format_RGB32);
                paint.drawImage(QPoint(0, 0), Mimg);
                core.NewPostImg=false;
            }
        }*/

    return a.exec();
}
