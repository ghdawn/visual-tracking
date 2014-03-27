#include <iostream>
#include"itrvision.h"
#include "itrbase.h"

using namespace std;

int main()
{
    itr_math::MathObjStandInit();
    char pathin[50]="bin/Debug/OUT%04d/%05d.pgm";
    char pathout[50]="bin/Debug/Descale%04d/%05d.pgm";
    char filein[50];
    char fileout[50];

    itr_vision::Scale* ScalObj;
    itr_vision::IOpnm* IOhelper;
    itr_math::Matrix ImgIn,ImgOut(320,240);
    int i=0,dir=0;
    while(1)
    {
        if(i==1000)
        {
            i=0;
            dir++;
        }
        sprintf(filein,pathin,dir,i);
        IOhelper->ReadPGMFile(filein,  ImgIn);

        ScalObj->SubSampling(ImgIn, ImgOut, 2);

        sprintf(fileout,pathout,dir,i);
        IOhelper->WritePGMFile(fileout, ImgOut);
        i++;
    }
    return 0;
}
