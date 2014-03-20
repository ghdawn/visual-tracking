#include <QtGui/QApplication>
#include "trackcore.h"
#include "joystickthread.h"
using namespace itr_tracker;
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
    joystick.Open("/dev/input/js1");
}
void JoyStickThread::run()
{
    while (!stopped)
    {
        joystick.Update();
        if(joystick.GetButtonStatus(0))
            core->Manual();
        else if(joystick.GetButtonStatus(1))
            core->BeginTrack();
        if(core->Tracking==true)
            gimbal.Control(joystick.GetAxisValue(0),joystick.GetAxisValue(1));
        usleep(100);
    }
}
void JoyStickThread::stop()
{
    stopped = true;
}
