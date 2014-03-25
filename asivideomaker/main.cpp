#include <iostream>
#include "itrvision.h"
#include "itrdevice.h"

using namespace std;

int main()
{
    itr_device::AsiCamera image_aquaire;

    const S32 height=320;
    const S32 weight=240;

    U8 Raw[height*weight];

    image_aquaire.Open(0,weight,height,0);

    itr_device::ICamera::AquairePara AqPara;

    AqPara.AquaireFreq=60;
    AqPara.Exposure=30*1000;
    AqPara.Gain=1;  ///

    image_aquaire.SetPara( AqPara);

    itr_device::ICamera::AquairePara* ExInfo;///NULL
    char path[50]="bin/Debug/OUT/%05d.pgm";
    char file[50]="bin/Debug/OUT/00298.pgm";
    char head[40];
//    char filename[10];

    sprintf(head,"P5\n%d %d\n255\n",weight,height);
    int i=0;
    while(image_aquaire.FetchFrame(Raw,weight*height,ExInfo))
    {

        sprintf(file, path, i);
        FILE* fout=fopen(file,"w");
        fprintf(fout,head);
        for(int j=0;j<weight*height;++j)
            fprintf(fout,"%c",Raw[j]);
        fclose(fout);
        ++i;
    }
    return 0;
}
