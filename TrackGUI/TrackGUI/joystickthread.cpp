#include <QtGui/QApplication>
#include "trackcore.h"
#include "joystickthread.h"
#include <iostream>
using std::cout;
using std::endl;
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
        F32 x;
        F32 y;
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
            core->posTrack.X = core->Width*0.5 - core->posTrack.Width*0.5;
            core->posTrack.Y = core->Height*0.5 - core->posTrack.Height*0.5;
            for(S32 i=0; i<axiscount; i++)
            {
                axisvalue[i]=joystick.GetAxisValue(i);
            }
            if( (axisvalue[4]>0.5)||(axisvalue[4]<-0.5))
            {
                core->posTrack.Height = core->posTrack.Height*(1-axisvalue[4]*0.01);
            }

            if( (axisvalue[3]>0.5)||(axisvalue[3]<-0.5))
            {
                core->posTrack.Width = core->posTrack.Width*(1+axisvalue[3]*0.01);
            }
            float dead=0.2f;
            if(axisvalue[0]>dead||axisvalue[0]<-dead)
            {
                x = axisvalue[0];
            }
            else
            {
                x = 0.0;
            }
            if(axisvalue[1]>dead||axisvalue[1]<-dead)
            {
                y = axisvalue[1];
            }
            else
            {
                y = 0.0;
            }

            core->gimbalControl.Control( x*100.0,y*100.0);

        }
        msleep(50);
    }
}
void JoyStickThread::stop()
{
    stopped = true;
    cout<<name.toStdString()<<" Stopped!\n";
}
