#ifndef TRACKCORE_H
#define TRACKCORE_H

#include "itrbase.h"
#include "itrvision.h"

class TrackCore
{
    public:
        /** Default constructor */
        TrackCore();
        void Init(const Matrix& input,const RectangleF& rect);
        void Go(const Matrix& input,RectangleF& rect);
    protected:
    private:
};

#endif // TRACKCORE_H
