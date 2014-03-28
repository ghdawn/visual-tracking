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
    return (sprintf((char*)buffer,"MO=%d",para));
}
S32 Commeder::com_PA(U8* buffer,F32 angle)
{
    if(angle<0||angle>360)
    {
        return -1;
    }
    S32 threadnum=(S32)(angle/360.0*tread_max);

    return (sprintf((char*)buffer,"PA=%d",threadnum));
}
S32 Commeder::com_JV(U8* buffer,F32 speed)
{
//    if(angle<0||angle>360)
//    {
//        return -1;
//    }
    S32 threadnum=(S32)(speed/360.0*tread_max);

    return (sprintf((char*)buffer,"JV=%d",threadnum));
}
S32 Commeder::com_BG(U8* buffer)
{
    return (sprintf((char*)buffer,"BG"));
}
S32 Commeder::com_ST(U8* buffer)
{
    return (sprintf((char*)buffer,"ST"));
}
