#include <QtGui/QApplication>
#include "trackcore.h"
#include "joystickthread.h"
#include <iostream>
using std::cout;
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
    isopen=joystick.Open("/dev/input/js1");
}
void JoyStickThread::run()
{
    while (isopen&&!stopped)
    {
        joystick.Update();
        if(joystick.GetButtonStatus(0))
        {
            core->Manual();
            core->posInit.Height = core->posInit.Height + 0.2*joystick.GetAxisValue(2);
            core->posInit.Width = core->posInit.Width + 0.2*joystick.GetAxisValue(3);
            gimbal.Control(joystick.GetAxisValue(0),joystick.GetAxisValue(1));
        }
        else if(joystick.GetButtonStatus(1))
        {
            core->BeginTrack();            
        }
        msleep(100);
    }
}
void JoyStickThread::stop()
{
    stopped = true;
    cout<<name.toStdString()<<" Stopped!\n";
}
