#include "trackcore.h"

namespace itr_tracker
{
    TrackCore::TrackCore()
    {
        kf.Init(4);
        F32 data[24]= {1,0,1,0,
                       0,1,0,1,
                       0,0,1,0,
                       0,0,0,1,
                      };
        kf.F_x.CopyFrom(data);
        postImg=NULL;
        posInit.Init(144,108,32,24);
        int missedImg=0;
    }

    TrackCore::~TrackCore()
    {
        if(postImg!=NULL)
            delete []postImg;
    }
    void TrackCore::Init(int Width,int Height)
    {
        current.Init(Width,Height);
        this->Width=Width;
        this->Height=Height;
        postImg=new U8[Width*Height];
    }

    void TrackCore::BeginTrack()
    {
        Tracking=true;
        TrackStatusChanged=true;
        kf.x[0]=posInit.X;
        kf.x[1]=posInit.Y;
        kf.x[2]=kf.x[3]=0;

        posTrack.X=posInit.X;
        posTrack.Y=posInit.Y;
        posTrack.Width=posInit.Width;
        posTrack.Height=posInit.Height;
    }

    void TrackCore::Manual()
    {
        Tracking=false;
        TrackStatusChanged=true;
    }
}
