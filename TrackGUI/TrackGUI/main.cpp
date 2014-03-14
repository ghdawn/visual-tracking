#include "mainwindow.h"
#include "itrbase.h"
#include "trackcore.h"
#include "trackthread.h"
#include "camerathread.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>
#include "processimage.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    itr_math::MathObjStandInit();
    printf("Hello\n");
    TrackCore core;
    core.Init(320,240);
    S32 i;

    QMutex mutexCurrent;
    QMutex mutexPost;

    CameraThread camera;
    camera.Init(&core);
    camera.mutexCurrent=&mutexCurrent;
    camera.mutexPost=&mutexPost;
    camera.start();
    TrackThread track;
    track.Init(&core);
    track.mutexCurrent =&mutexCurrent;
    track.start();
    MainWindow w;
    w.resize(320,240);
    w.core=&core;
    w.show();
    return a.exec();
}
