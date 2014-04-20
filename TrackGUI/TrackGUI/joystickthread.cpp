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

        }
        else if(joystick.GetButtonStatus(1))
        {
            core->BeginTrack();            
        }
        if(core->Tracking)
        {
            for(S32 i=0; i<axiscount; i++)
            {
                axisvalue[i]=JoyStickPro.GetAxisValue(i);
                if(axisvalue[4]>0.5||axisvalue[4]<-0.5)
                {
                    core->posInit.Height = core->posInit.Height*(1-axisvalue[4]*0.001);

                }

                if(axisvalue[3]>0.5||axisvalue[3]<-0.5)
                {
                    core->posInit.Width = core->posInit.Width*(1+axisvalue[3]*0.001);

                }
            }

//            if(axisvalue[2]>0.8)
//            {
//                core->posInit.Width = 1.2*core->posInit.Width;
//            }
//            else if(axisvalue[2]>0.6&&axisvalue[2]<0.8)
//            {
//                core->posInit.Width = 1.1*core->posInit.Width;
//            }
//            else if(axisvalue[2]>-0.8&&axisvalue[2]<-0.6)
//            {
//                core->posInit.Width = 0.9*core->posInit.Width;
//            }
//            else if(axisvalue[2]<-0.8)
//            {
//                core->posInit.Width = 0.8*core->posInit.Width;
//            }

//            if(axisvalue[3]>0.8)
//            {
//                core->posInit.Height = 1.2*core->posInit.Height;
//            }
//            else if(axisvalue[3]>0.6&&axisvalue[3]<0.8)
//            {
//                core->posInit.Height = 1.1*core->posInit.Height;
//            }
//            else if(axisvalue[3]>-0.8&&axisvalue[3]<-0.6)
//            {
//                core->posInit.Height = 0.9*core->posInit.Height;
//            }
//            else if(axisvalue[3]<-0.8)
//            {
//                core->posInit.Height = 0.8*core->posInit.Height;
//            }

           // gimbal.Control(joystick.GetAxisValue(0),joystick.GetAxisValue(1));
        }
        msleep(100);
    }
}
void JoyStickThread::stop()
{
    stopped = true;
    cout<<name.toStdString()<<" Stopped!\n";
}
