#include <QtGui/QApplication>
#include "trackcore.h"
#include "joystickthread.h"
using namespace itr_tracker;
using namespace itr_device;
JoyStickThread::JoyStickThread(QString name )
{
    stopped = false;
    this->name = name;
}
JoyStickThread::~JoyStickThread()
{

}
void JoyStickThread::Init(TrackCore* core)
{

    this->core = core;
    joystick.Open("/dev/input/js0");
}
void JoyStickThread::run()
{
    while (!stopped)
    {
        joystick.Update();
        if(joystick.GetButtonStatus(0))
            core->Manual();

        if(joystick.GetButtonStatus(1))
            core->BeginTrack();
        gimbal.Control(joystick.GetAxisValue(0),joystick.GetAxisValue(1));
    }
}
void JoyStickThread::stop()
{
    stopped = true;
}
