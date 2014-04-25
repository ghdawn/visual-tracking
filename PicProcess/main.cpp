#include <iostream>
#include"itrvision.h"
#include "itrbase.h"
#include "serialport.h"
#include "gimbal.h"

using namespace std;
void GetData(S32* buffer,S32 &length)
{
    length=0;
    string str;
    getline(cin,str);
    S32 commer1=0,commer2=0;
    int a,b,c;

    do
    {

        if(str[commer2]==' ')
        {

            sscanf(&str[commer1],"%d,%d,%d",&a,&b,&c);
            buffer[length]=a+b+c;
            length++;

            commer2++;
            commer1=commer2;

        }
        else
        {

            commer2++;
        }

    }
    while(str[commer2]!='\0');

    sscanf(&str[commer1],"%d,%d,%d",&a,&b,&c);
    buffer[length]=a+b+c;
    length++;
}
int main()
{
    itr_math::MathObjStandInit();
    S32 buffer[100];
    S32 length=0;
//    U8 buf[15];
//    SerialPort sp;
//    sp.Init("/dev/ttyUSB0",115200);
//    for(int i=0;i<10;i++)
//        buf[i]=i+'0';
//    sp.send(buf,10);
//    sp.receive(buf,5);
//    printf("%s\n",buf);
//    sp.receive(buf,10);
//    printf("%s\n",buf);

    itr_tracker::Gimbal gimbal;
    gimbal.Init(1,1,1,1);
    gimbal.Control(1,2);
    getchar();
    while(1)
    {
    GetData(buffer,length);
    for(S32 t=0; t<length; t++)
    {
        printf("%d\t",buffer[t]);

    }
    printf("\n");
    }
    getchar();
    getchar();
    char pathin[50]="bin/Debug/pic/OUT%04d/%05d.pgm";
    char pathout[50]="bin/Debug/pic/OUT%d/%05d.pgm";
    char filein[50];
    char fileout[50];
/// ///////////////////////////////
//    F32 x,y,z;
//   F32 tmp;
//    while(1)
//    {
//        itr_math::NumericalObj->Rand(tmp);
//        x=tmp*100;
//        itr_math::NumericalObj->Rand(tmp);
//        y=tmp*100;
//        itr_math::NumericalObj->Rand(tmp);
//        z=tmp*100;
//        printf("POS :\tx:%04f\ty:%4f\tz:%4f\n",x,y,z);
//        for(S32 con=0;con<30000000;con++)
//        ;
//    }
/// ///////////////////////////////
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

