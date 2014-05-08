#include <iostream>
#include "itrbase.h"
#include "itrvision.h"
#include "lktracking.h"

using namespace std;

int main()
{
    itr_math::MathObjStandInit();
    Matrix imgInput,imgOutput,imgObject;
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/input.pgm",imgInput);
    itr_math::Point2D center(imgInput.GetCol()/2,imgInput.GetRow()/2);
    itr_math::Point2D pin,pout;
    F32 xmax,xmin,ymax,ymin;

    //imgOutput.Init(ymax-ymin,xmax-xmin);
    F32 offsetX,offsetY;
    itr_math::RectangleS rect(0,0,imgInput.GetCol(),imgInput.GetRow());
    itr_vision::Rotate::rotate(imgInput,rect,30,imgOutput);
    itr_vision::IOpnm::WritePGMFile("bin/Debug/output2.pgm",imgOutput);

    Matrix imgAffine(imgInput.GetRow(),imgInput.GetCol());
    F32 data[]={0.9,0.05,10
               ,0.05,0.9,-10,
                 0,0,1};
    Matrix transMatrix(3,3,data);
    Vector p(3),q(3);
    for(int j=0;j<imgInput.GetRow();j++)
    {
        for(int i=0;i<imgInput.GetCol();i++)
        {
            p[0]=i;
            p[1]=j;
            p[2]=1;
            q=transMatrix*p;
            imgAffine(q[1],q[0])=imgInput(j,i);
        }
    }
     itr_vision::IOpnm::WritePGMFile("bin/Debug/output3.pgm",imgAffine);
    itr_math::RectangleF rectF(0,0,rect.Width,rect.Height);
    printf("%d %d\n", rect.Width,rect.Height);
    lktracking tracker;
    tracker.Init(imgInput,rectF);
    F32 x,y;
    tracker.Go(imgAffine,rectF,x,y);
    return 0;
}
