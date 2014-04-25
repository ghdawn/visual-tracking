#include "trackcore.h"

namespace itr_tracker
{
    TrackCore::TrackCore()
    {
        kf.Init(4);
        F32 data[16]= {1,0,1,0,
                       0,1,0,1,
                       0,0,1,0,
                       0,0,0,1
                      };
        kf.F_x.CopyFrom(data);
        kf.F_n.SetDiag(1);
        kf.F_n(0,0)=kf.F_n(1,1)=0.5;
        postImg=NULL;
        posInit.Init(144,108,36,27);
        int missedImg=0;
        NewTrackImg=false;
        NewPostImg=false;
        gimbalControl.Init(1,1,1,1);
    }

    TrackCore::~TrackCore()
    {
        if(postImg!=NULL)
            delete []postImg;
    }
    void TrackCore::Init(int Width,int Height)
    {
        current.Init(Height,Width);
        this->Width=Width;
        this->Height=Height;
        postImg=new U8[Width*Height*4];
    }

    void TrackCore::BeginTrack()
    {
        TrackStatusChanged=true;
        Tracking=true;
        kf.x[0]=posTrack.X;
        kf.x[1]=posTrack.Y;
        kf.x[2]=kf.x[3]=0;
    }

    void TrackCore::Manual()
    {
        TrackStatusChanged=true;
        Tracking=false;

        posTrack.X=posInit.X;
        posTrack.Y=posInit.Y;
        posTrack.Width=posInit.Width;
        posTrack.Height=posInit.Height;
    }
}
