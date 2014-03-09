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
        preImg=NULL;
        postImg=NULL;
        posInit.Init(144,108,32,24);
    }

    TrackCore()::~TrackCore()
    {
        if(preImg!=NULL)
            delete []preImg;
        if(postImg!=NULL)
            delete []postImg;
    }
    void TrackCore::Init(int ImageSize)
    {
        preImg=new U8[ImageSize];
        postImg=new U8[ImageSize];
    }

    void BeginTrack()
    {
        Tracking=true;

        kf.X[0]=posInit.X;
        kf.X[1]=posInit.Y;
        kf.X[2]=kf.X[3]=0;

        posTrack.X=posInit.X;
        posTrack.Y=posInit.Y;
        posTrack.Width=posInit.Width;
        posTrack.Height=posInit.Height;
    }

    void Manual()
    {
        Tracking=false;
    }
}
