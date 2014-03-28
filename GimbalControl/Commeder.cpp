#include "Commeder.h"
#include <stdio.h>

#define tread_max 2048*4

Commeder::Commeder()
{
    //ctor
}

Commeder::~Commeder()
{
    //dtor
}
S32 Commeder::com_MO(U8* buffer,S32 para)
{
    if(para!=0&&para!=1)
    {
        return -1;
    }

    sprintf((char*)buffer,"MO=%d",para);

    return 5;
}
S32 Commeder::com_PA(U8* buffer,F32 angle)
{
    if(angle<0||angle>360)
    {
        return -1;
    }
    F32 length=0;
    F32 threadnum=(angle/360.0*tread_max);
    itr_math::NumericalObj->Log10(threadnum,length);
    S32 Length=0;
    itr_math::NumericalObj->Ceil(length,Length);

    sprintf((char*)buffer,"PA=%d",(S32)threadnum);

    return (Length+3);
}
S32 Commeder::com_BG(U8* buffer)
{
    sprintf((char*)buffer,"BG");

    return (2);
}
