#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include <QMutex>
using namespace itr_tracker;
class CameraThread : public QThread
{
public:
    CameraThread(QString name = "")
    {
        stopped = false;
        this->name = name;
    }
    void Init(TrackCore* core)
    {

        this->core = core;
    }

    void run()
    {
        while (!stopped)
        {

        }
        qDebug("end!");
    }

    void stop()
    {
        mutex->lock();
        stopped = true;
        mutex->unlock();
    }
    QMutex* mutex;
private:
    volatile bool stopped;
    QString name;
    TrackCore *core;
};
