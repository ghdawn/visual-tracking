#ifndef COMMEDER_H
#define COMMEDER_H
#include "itrbase.h"

class Commeder
{
    public:
        Commeder();
        virtual ~Commeder();
        //mo
        //pa
<<<<<<< HEAD
        //bg
        S32 com_MO(U8* buffer,S32 para);
        S32 com_PA(U8* buffer,F32 angle);
        S32 com_BG(U8* buffer);
=======
        //bg stop jv
        S32 com_MO(U8* buffer,S32 para);
        S32 com_PA(U8* buffer,F32 angle);
        S32 com_BG(U8* buffer);
        S32 com_ST(U8* buffer);
        S32 com_JV(U8* buffer,F32 speed);
>>>>>>> 9989ac046ecc889a1f9429e8132a486a867a4fd3
    protected:
    private:

};

#endif // COMMEDER_H
