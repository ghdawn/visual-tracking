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
        U8 *buffer;
        joystick.Update();
        S32 axiscount=0;
        axiscount=joystick.GetAxisCount();
        F32 axisvalue[30]={0};

        if(joystick.GetButtonStatus(0))
        {
            core->Manual();
        }
        else if(joystick.GetButtonStatus(1))
        {
            core->BeginTrack();            
        }

        if(!core->Tracking)
        {
            for(S32 i=0; i<axiscount; i++)
            {
                axisvalue[i]=joystick.GetAxisValue(i);
            }

            if(axisvalue[4]>0.5||axisvalue[4]<-0.5)
            {
                core->posInit.Height = core->posInit.Height*(1-axisvalue[4]*0.001);
            }

            if(axisvalue[3]>0.5||axisvalue[3]<-0.5)
            {
                core->posInit.Width = core->posInit.Width*(1+axisvalue[3]*0.001);
            }

            if(axisvalue[0]>0.5||axisvalue[0]<-0.5)
            {
                ASF32(&buffer[1]) = axisvalue[0];
            }

            if(axisvalue[1]>0.5||axisvalue[1]<-0.5)
            {
                ASF32(&buffer[2]) = axisvalue[1];
            }

            gimbal.Control( ASF32(&buffer[1]), ASF32(&buffer[2]));
        }
        msleep(100);
    }
}
void JoyStickThread::stop()
{
    stopped = true;
    cout<<name.toStdString()<<" Stopped!\n";
}
