#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include <QMutex>
using namespace itr_tracker;
class TrackThread : public QThread
{
public:
    TrackThread(QString name = "");
    void Init(TrackCore* core);
    void run();
    void stop();
    ~TrackThread();
    QMutex* mutextrack;
    QMutex* mutexPreImg;
private:
    volatile bool stopped;
    QString name;
    TrackCore *core;
    lktracking *tracking;
    Matrix Hv,R;
    Vector z;
};
