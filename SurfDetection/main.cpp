#include <iostream>
#include "itrvision.h"
//#include "stdlib.h"
using namespace std;
using namespace itr_vision;

S32 track_surf(std::vector<VectorFeaturePoint> FeaturePointList, itr_math::RectangleS rect_soulth,Matrix mat2,itr_math::RectangleS &rect_result)
{
    /// copy interested area to a small matrix
//    itr_math::Matrix mat_tmp(rect_soulth.Height,rect_soulth.Width);
//    mat1.CopyTo(    (S32)rect_soulth.Y,
//                    (S32)rect_soulth.X,
//                    (S32)rect_soulth.Width,
//                    (S32)rect_soulth.Height,
//                    (F32*)mat_tmp.GetData());

    /// calculate featurepoint of iterested area


    /// calculate featurepoint of target picture
    SURF surf2;

    S32 w2=mat2.GetCol();
    S32 h2=mat2.GetRow();
    mat2.AllMul(1/255.f);

    surf2.Init(w2,h2,5,4,2,0.0004f);
    std::vector<VectorFeaturePoint> FeaturePointList2;
    surf2.ProcessAll(mat2, FeaturePointList2);

    /// feature match
    FeatureMatch feature_matchobj;
    S32 matchnum=0;

//    /// ////////////////////////
//    S32 m_tmp=FeaturePointList.size();
//    S32 n_tmp=FeaturePointList2.size();
//    /// ////////////////////////

    feature_matchobj.FeatureMatchDo( FeaturePointList, FeaturePointList2,  matchnum);

    S32 size_feature =FeaturePointList.size();
    F32* off_x=new F32[matchnum]();
    F32* off_y=new F32[matchnum]();
    ///find target rectangle
    if(matchnum>0)
    {
        S32 j=0;
//        RectangleS rect_point1(100,100,10,10);
//        RectangleS rect_point2(100,100,10,10);
        for(S32 i=0; i<size_feature; i++)
        {
            if(FeaturePointList[i].ID!=-1&&FeaturePointList[i].Quality)
            {
                off_x[j]=FeaturePointList2[FeaturePointList[i].ID].X-FeaturePointList[i].X;
                off_y[j]=FeaturePointList2[FeaturePointList[i].ID].Y-FeaturePointList[i].Y;
                j++;

//                rect_point1.X=FeaturePointList[i].X;
//                rect_point1.Y=FeaturePointList[i].Y;
//                rect_point2.X=FeaturePointList2[FeaturePointList[i].ID].X;
//                rect_point2.Y=FeaturePointList2[FeaturePointList[i].ID].Y;
//
//                itr_vision::Draw::Rectangle(mat1, rect_point1, 1);
//                itr_vision::Draw::Rectangle(mat2, rect_point2, 1);

            }

        }


        if(j>1)
        {
            for(S32 z=1; z<j; z++)
            {
                off_x[0]+=off_x[z];
                off_y[0]+=off_y[z];
            }
            off_x[0]/=j;
            off_y[0]/=j;
        }
        rect_result.X=rect_soulth.X+off_x[0];
        rect_result.Y=rect_soulth.Y+off_y[0];
        rect_result.Width=rect_soulth.Width;
        rect_result.Height=rect_soulth.Height;


//        itr_vision::Draw::Rectangle(mat1, rect_soulth, 1);
//        itr_vision::Draw::Rectangle(mat2, rect_result, 1);
//        mat2.AllMul(255);
//        mat1.AllMul(255);
//        itr_vision::IOpnm::WritePGMFile("trackout1.pgm", mat1);
//        itr_vision::IOpnm::WritePGMFile("trackout2.pgm", mat2);
        return 1;
    }
    else
    {
        return 0;
    }
}


int main()
{
    itr_math::MathObjStandInit();


    char path[50]="bin/Debug/01_david/pgm/%05d.pgm";
    char file[50]="bin/Debug/01_david/pgm/00001.pgm";
    FILE* fout=fopen("bin/Debug/result.txt","w");
    Matrix current,last;
    IOpnm::ReadPGMFile(file, last);

    ///计算首张图片特征点
    SURF surf;

    S32 w=last.GetCol();
    S32 h=last.GetRow();
    last.AllMul(1/255.f);

    surf.Init(w,h,5,4,2,0.0004f);
    std::vector<VectorFeaturePoint> FeaturePointList;
    surf.ProcessAll(last, FeaturePointList);
    S32 feature_num=FeaturePointList.size();

    /// 读取原始感兴趣区域
    RectangleS rect(0, 0, 0, 0);
    FILE *InitInfo=fopen("bin/Debug/01_david/init.txt","r");
    fscanf(InitInfo,"%d %d %d %d",&rect.X,&rect.Y,&rect.Width,&rect.Height);
    fclose(InitInfo);
    rect.Width-=rect.X;
    rect.Height-=rect.Y;

    ///筛选感兴趣区域点
    for(S32 i=0; i<feature_num; i++)
    {
        if(rect.IsInRectangle(FeaturePointList[i].X,FeaturePointList[i].Y))
        {
            FeaturePointList[i].Quality=1;
        }
        else
        {
            FeaturePointList[i].Quality=0;
        }
    }

    RectangleS rect_result(0,0,0,0);

    for(S32 i=2; i<1000; i++)
    {
        sprintf(file, path, i);
        IOpnm::ReadPGMFile(file, current);

        if(track_surf(FeaturePointList,  rect, current,rect_result))
        {
            itr_vision::Draw::Rectangle(current,rect_result,255);

            sprintf(file,"bin/Debug/output/%05d.pgm",i);
            itr_vision::IOpnm::WritePGMFile(file,current);

            fprintf(fout,"%d %d %d %d\n",rect_result.X,rect_result.Y,rect_result.X+rect_result.Width,rect_result.Y+rect_result.Height);
        }
        else
        {
            printf("\nerror! not matched:\t%d\n",i);
        }
    }
    fclose(fout);
    return 0;
}
