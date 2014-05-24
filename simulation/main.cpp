#include <iostream>
#include "itrbase.h"
#include "itrvision.h"
#include "lktracking.h"

using namespace std;

void Affine_Transform()
{

    Matrix imgInput,imgOutput,imgObject;
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/background2.pgm",imgInput);
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
}

void Generate(const Matrix&imgBackground,const Matrix& imgObject,int bx,int by,int ox,int oy,int num )
{
    Matrix imgResult(240,320);
    itr_math::RectangleS rectBackground(bx,by,320,240);
    itr_vision::Pick::Rectangle(imgBackground,rectBackground,imgResult);
    itr_math::Point2D startPoint(ox,oy);
    float noise;
    for(int j=0;j<imgObject.GetRow();j++)
    {
        for(int i=0;i<imgObject.GetCol();i++)
        {
            itr_math::NumericalObj->Rand(noise);
            imgResult(startPoint.Y+j,startPoint.X+i)=imgObject(j,i);
        }
    }
    float* fp=imgResult.GetData();
    for(int i=0;i<240*320;i++)
    {
        itr_math::NumericalObj->RandGaussian(noise);
        *fp+=noise;
        fp++;
    }

    char filename[20];
    sprintf(filename,"bin/Debug/sim%03d.pgm",num);
    itr_vision::IOpnm::WritePGMFile(filename,imgResult);
}
void Ground_Truth()
{
    int x=160,y=120;
    Matrix imgBackground,imgBackground2,imgObject;
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/object.pgm",imgObject);
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/background.pgm",imgBackground);
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/output3.pgm",imgBackground2);
    Generate(imgBackground,imgObject,200,200,x,y,0);
    int vx,vy;

    FILE* fp=fopen("gt.txt","w");
    FILE* loc=fopen("pos.txt","w");
    fprintf(loc,"%d %d\n",x,y);
    for(int i=1;i<100;i++)
    {
        itr_math::NumericalObj->Rand(-5,5,vx);
        itr_math::NumericalObj->Rand(-5,5,vy);
        x+=vx;
        y+=vy;
        fprintf(fp,"%d %d\n",vx,vy);
        fprintf(loc,"%d %d\n",x,y);
        Generate(imgBackground,imgObject,200+i*2,200+i,x,y,i);
    }
    fclose(fp);
    fclose(loc);
}

void SimTest()
{
    Matrix img1,img2;
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/sim000.pgm",img1);
    itr_vision::IOpnm::ReadPGMFile("bin/Debug/sim000.pgm",img2);
    itr_vision::ConvoluteSquare conv;
    conv._KLTComputeSmoothedImage(img1, 3, img2);

    lktracking tracker;

    F32 vx,vy;
    int x,y;
    FILE* fp=fopen("track.txt","w");
    FILE* loc=fopen("pos.txt","r");
    fscanf(loc,"%d %d\n",&x,&y);
    itr_math::RectangleF rectF(x-5,y-5,50,50);
     tracker.Init(img2,rectF);
    for(int i=1;i<100;i++)
    {
        char filename[20];
        sprintf(filename,"bin/Debug/sim%03d.pgm",i);
        itr_vision::IOpnm::ReadPGMFile(filename,img1);
        conv._KLTComputeSmoothedImage(img1, 3, img2);
        fscanf(loc,"%d %d\n",&x,&y);
        rectF.X=x-5;
        rectF.Y=y-5;
        tracker.Go(img2,rectF,vx,vy);
        fprintf(fp,"%f %f\n",vx,vy);
    }
    fclose(fp);

}
int main()
{
    itr_math::MathObjStandInit();
    Affine_Transform();
    Ground_Truth();
    cin.get();
    SimTest();
    return 0;
}
