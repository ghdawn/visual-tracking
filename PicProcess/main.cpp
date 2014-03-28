#include <iostream>
#include"itrvision.h"
#include "itrbase.h"

using namespace std;

int main()
{
    itr_math::MathObjStandInit();
    char pathin[50]="bin/Debug/pic/OUT%04d/%05d.pgm";
    char pathout[50]="bin/Debug/pic/OUT%d/%05d.pgm";
    char filein[50];
    char fileout[50];

    itr_math::Matrix ImgIn,ImgOut(240,320);
    int i=1,dir=2;
    while(1)
    {
        if(i==1000)
        {
            i=1;
            dir++;
        }
        sprintf(filein,pathin,dir,i);
        itr_vision::IOpnm::ReadPGMFile(filein,  ImgIn);
        itr_vision::Scale::SubSampling(ImgIn, ImgOut, 2);
        sprintf(fileout,pathout,dir,i);
        itr_vision::IOpnm::WritePGMFile(fileout, ImgOut);
        i++;
    }
    return 0;
}
