#ifndef TRACKCORE_H
#define TRACKCORE_H

#include "itrbase.h"
#include "itrvision.h"
#include "lktracking.h"
namespace itr_tracker
{
    class TrackCore
    {
        public:
            /** Default constructor */
            TrackCore();
            void Init(const Matrix &input,const RectangleF &rect);
            void Go(const Matrix &input,RectangleF &rect);
        protected:
        private:
        Matrix current,last;
        RectangleF rect;
        KalmanFilter kf;
        Matrix Hx,Hv,R;
        Vector z,X,v;
        lktracking tracking;
        F32 _x,_y,_u=0,_v=0;
    };
}
#endif // TRACKCORE_H
