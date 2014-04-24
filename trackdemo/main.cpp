#include "lktracking.h"
#include "detection.h"
#include <iostream>

using namespace std;

int main()
{
    itr_math::MathObjStandInit();
    char dir[50]="bin/Debug/09_carchase";
    char path[50]="%s/pgm/%05d.pgm";

    char file[50];


    ///读取初始位置
    RectangleF rect(0, 0, 0, 0);
    sprintf(file,"%s/%s",dir,"init.txt");
    FILE *InitInfo=fopen(file,"r");
    fscanf(InitInfo,"%f,%f,%f,%f",&rect.X,&rect.Y,&rect.Width,&rect.Height);
    fclose(InitInfo);
    rect.Width-=rect.X;
    rect.Height-=rect.Y;

    ///读取初始图像
    int beginindex=1;
    FILE *fout=fopen("bin/Debug/result.txt","w");
    Matrix current,last;
    sprintf(file,path,dir,beginindex);
    IOpnm::ReadPGMFile(file, current);
    IOpnm::ReadPGMFile(file, last);

    ///卡尔曼滤波用
    KalmanFilter kf;
    kf.Init(4);
    F32 data[24]= {1,0,1,0,
                   0,1,0,1,
                   0,0,1,0,
                   0,0,0,1,
                   1,0,0,0,
                   0,1,0,0
                  };
    kf.F_x.CopyFrom(data);
    kf.F_n.SetDiag(1);
    kf.F_n(0,0)=kf.F_n(1,1)=0.5;

    Matrix Hx(2,4),Hv(2,4),R(2,2);
    R.SetDiag(1.012306);
    Hx.CopyFrom(data+16);
    Hv.CopyFrom(data+8);
    Vector z(2),X(4),v(2);
    kf.x[0]=rect.X;
    kf.x[1]=rect.Y;
    kf.x[2]=0;
    kf.x[3]=0;

    F32 _x,_y,_u=0,_v=0;

    lktracking tracking;
    tracking.Init(current,rect);
    RectangleS rectout;
    rectout.X=rect.X;
    rectout.Y=rect.Y;
    rectout.Width=rect.Width;
    rectout.Height=rect.Height;
    Detection detect(current,rectout,15);

    for(int k=beginindex; k<1000; k+=1)
    {
        sprintf(file, path,dir, k);
        printf("%s\n\n",file);
        last=current;
        IOpnm::ReadPGMFile(file, current);
        X=kf.UpdateModel();

        if(tracking.Go(current,rect,_u,_v))
        {
            z[0]=_u;
            z[1]=_v;
            X=kf.UpdateMeasure(Hv,R,z);

        }
        rectout.X=rect.X;
        rectout.Y=rect.Y;

        if(detect.Go(current,rectout))
        {
            z[0]=rectout.X;
            z[1]=rectout.Y;
            X=kf.UpdateMeasure(Hx,R,z);

        }
        rect.X=X[0];
        rect.Y=X[1];
        if(true)
        {
            RectangleS rectout;
            rectout.X=rect.X;
            rectout.Y=rect.Y;
            rectout.Width=rect.Width;
            rectout.Height=rect.Height;

            Draw::Rectangle(current,rectout,255);
            sprintf(file,"bin/Debug/output/%05d.pgm",k);
            IOpnm::WritePGMFile(file,current);
            fprintf(fout,"%f %f %f %f\n",rect.X,rect.Y,rect.X+rect.Width,rect.Y+rect.Height);
        }

    }
    fclose(fout);
    return 0;
}
