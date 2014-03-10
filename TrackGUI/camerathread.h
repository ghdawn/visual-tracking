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
    CameraThread(QString name = "");
    void Init(TrackCore* core);
    void run();
    void stop();
    ~CameraThread();
    QMutex* mutex;
private:
    volatile bool stopped;
    QString name;
    TrackCore *core;
};
