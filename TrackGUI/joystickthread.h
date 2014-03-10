#include <iostream>
#include <QApplication>
#include <QThread>
#include <QString>
#include "trackcore.h"
#include "itrdevice.h"
#include "stdjoystick.h"
#include "gimbal.h"
#include <QMutex>
using namespace itr_tracker;
using namespace itr_device;
class JoyStickThread : public QThread
{
public:
    JoyStickThread(QString name = "");
    ~JoyStickThread();
    void Init(TrackCore* core);
    void run();
    void stop();

    QMutex* mutex;
    TrackCore *core;
    volatile bool stopped;
private:
    QString name;
    StdJoyStick joystick;
    Gimbal gimbal;
};
