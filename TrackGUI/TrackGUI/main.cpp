#include "mainwindow.h"
#include "itrbase.h"
#include "trackcore.h"
#include "trackthread.h"
#include "camerathread.h"
#include "joystickthread.h"
#include <QApplication>
#include <QtCore>
#include <QObject>
#include <QtGui>
#include "processimage.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    itr_math::MathObjStandInit();

    TrackCore core;
    core.Init(320,240);
    core.Tracking=false;
    core.TrackStatusChanged=false;

    QMutex mutexCurrent;
    QMutex mutexPost;

    CameraThread camera("Camera");
    camera.Init(&core);
    camera.mutexCurrent=&mutexCurrent;
    camera.mutexPost=&mutexPost;
    camera.start();

    TrackThread track("Track");
    track.Init(&core);
    track.mutexCurrent =&mutexCurrent;
    track.mutexPost=&mutexPost;
    track.start();

    JoyStickThread joystick("Joystick");
    joystick.Init(&core);
    joystick.start();

    MainWindow w;
    w.resize(core.Width,core.Height);
    w.mutexPost=&mutexPost;
    w.setAttribute(Qt::WA_OpaquePaintEvent);
    w.core=&core;
    w.show();

    /*QObject::connect( &w,SIGNAL(close()), &camera, SLOT(stop()));
    QObject::connect( &w,SIGNAL(close()), &track, SLOT(stop()));
    QObject::connect( &w,SIGNAL(close()), &joystick, SLOT(stop()));
    QObject::connect( &a,SIGNAL(lastWindowClosed()), &camera, SLOT(stop()));*/
    return a.exec();
}
