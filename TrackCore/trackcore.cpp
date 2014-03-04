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
                       1,0,0,0,
                       0,1,0,0
                      };
        kf.F_x.CopyFrom(data);
        Hx.Init(2,4);
        Hv.Init(2,4);
        R.Init(2,2);
        R.SetDiag(30.012306);
        Hx.CopyFrom(data+16);
        Hv.CopyFrom(data+8);
        z.Init(2),X.Init(4),v.Init(2);
    }

    void TrackCore::Init(const Matrix &input,const RectangleF &Rect)
    {
        rect.X=Rect.X;
        rect.Y=Rect.Y;
        rect.Width=Rect.Width;
        rect.Height=Rect.Height;
        kf.x[0]=rect.X;
        kf.x[1]=rect.Y;
        kf.x[2]=0;
        kf.x[3]=0;
//        current=input;
        tracking.Init(input,rect);
    }
    void TrackCore::Go(const Matrix &input,RectangleF &rect)
    {
//        last=current;
//        current=input;
        X=kf.UpdateModel();

        if(tracking.Go(input,rect,_u,_v))
        {
            z[0]=_u;
            z[1]=_v;
            X=kf.UpdateMeasure(Hv,R,z);

        }
        rect.X=X[0];
        rect.Y=X[1];
    }
}
