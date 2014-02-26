#ifndef CAMERA_H
#define CAMERA_H
#include "itrbase.h"
using itr_math::Matrix;

class ImageAquaire
{
    public:
        /** Default constructor */
        ImageAquaire();
        void Init(int Width,int Height);
        /**
        * \brief 获取一帧图像
        * \param 输出的灰度图像
        */
        void FetchFrame(Matrix &img);

        void Stop();
    protected:
    private:
};

#endif // CAMERA_H
