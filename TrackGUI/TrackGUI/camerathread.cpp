#include <QtGui/QApplication>
#include "camerathread.h"
#include "trackcore.h"
#include "itrvision.h"
#include <QtGui>
#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
using std::cout;
using std::stringstream;
CameraThread::CameraThread(QString name )
{
    stopped = false;
    this->name = name;

}
CameraThread::~CameraThread()
{
    delete[] inputimg;
    delete[] rawImg;
}
void CameraThread::Init(TrackCore *core)
{
    this->core = core;
    length=core->Width*core->Height;
    inputimg=new U8[length*4];
    rawImg=new U8[640*480];
    imgBuffer.Init(480,640);
    process.Init(core->Width,core->Height);
    camera.Open(0,640,480,0);
    camera.Start();
    clock.Tick();
}
void CameraThread::run()
{
    int delta;
    int img=0,dir=0;
    char filename[20];
    Matrix Q(4,4);
    float sigma=0.0001;
    Q.SetDiag(sigma);
    Q(0,0)=Q(1,1)=0;
    Vector n(4),X(4);
    while (!stopped)
    {
        infolist.clear();
        memset(rawImg,0,sizeof(rawImg));
        camera.FetchFrame(rawImg,640*480,exinfo);

        delta=clock.Tick();

        core->deltaT+=delta;
        for(int i=0;i<640*480;i++)
            imgBuffer[i]=rawImg[i];
        if(mutexCurrent->tryLock())
        {
            itr_vision::Scale::Bilinear(imgBuffer,core->current);
            core->NewTrackImg=true;
            mutexCurrent->unlock();
        }

        if(!core->Tracking)
        {
            info="No tracker!!";
            infolist.push_back(info);
        }
        else
        {
            core->kf.F_x(0,2)=core->kf.F_x(1,3)=delta;
            core->kf.F_n(2,2)=delta;
            core->kf.F_n(3,3)=delta;
            itr_math::NumericalObj->RandGaussian(n[1]);
            itr_math::NumericalObj->RandGaussian(n[3]);
            n[1]*=sigma;
            n[3]*=sigma;
            core->kf.UpdateModel(Q,n);
            core->posTrack.X=core->kf.x[0];
            core->posTrack.Y=core->kf.x[1];
            X=core->kf.x;
            X[0]+=core->posTrack.Width/2;
            X[1]+=core->posTrack.Height/2;
            core->gimbalControl.Control(X);
            stringstream ss;
            ss<<"X:"<<core->posTrack.X+core->posTrack.Width/2
                    <<"\nY:"<<core->posTrack.Y+core->posTrack.Height/2;
            ss<<"\nCamera:"<<1000/delta<<"Hz";
            for(int i=0;i<3;i++)
            {
                ss>>info;
                infolist.push_back(info);
            }
        }

        rectangle.X=core->posTrack.X;
        rectangle.Y=core->posTrack.Y;
        rectangle.Width=core->posTrack.Width;
        rectangle.Height=core->posTrack.Height;

        if(mutexPost->tryLock())
        {
            for(int i=0;i<length;++i)
            {
                inputimg[4*i]=core->current[i];
                inputimg[4*i+1]=core->current[i];
                inputimg[4*i+2]=core->current[i];
                inputimg[4*i+3]=core->current[i];
            }
            process.Process(inputimg,rectangle,infolist,core->postImg);
            core->NewPostImg=true;
            mutexPost->unlock();
        }
        //printf("End Draw Post!\n");
    }
}
void CameraThread::stop()
{
    stopped = true;
    cout<<name.toStdString()<<" Stopped!\n";
}

