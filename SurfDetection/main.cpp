#include <iostream>
#include "itrvision.h"
//#include "stdlib.h"
using namespace std;
using namespace itr_vision;

S32 track_surf(Matrix mat1, itr_math::RectangleS rect_soulth,Matrix mat2,itr_math::RectangleS &rect_result)
{
    /// copy interested area to a small matrix
//    itr_math::Matrix mat_tmp(rect_soulth.Height,rect_soulth.Width);
//    mat1.CopyTo(    (S32)rect_soulth.Y,
//                    (S32)rect_soulth.X,
//                    (S32)rect_soulth.Width,
//                    (S32)rect_soulth.Height,
//                    (F32*)mat_tmp.GetData());

    /// calculate featurepoint of iterested area
    SURF surf;

    S32 w=mat1.GetCol();
    S32 h=mat1.GetRow();
    mat1.AllMul(1/255.f);

    surf.Init(w,h,5,4,2,0.0004f);
    std::vector<VectorFeaturePoint> FeaturePointList;
    surf.ProcessAll(mat1, FeaturePointList);

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
        RectangleS rect_point1(100,100,10,10);
        RectangleS rect_point2(100,100,10,10);
        for(S32 i=0; i<size_feature; i++)
        {
            if(FeaturePointList[i].ID!=-1&&rect_soulth.IsInRectangle(FeaturePointList[i].X,FeaturePointList[i].Y))
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


        if(j>0)
            for(S32 z=1; z<j; z++)
            {
                off_x[0]+=off_x[z];
                off_y[0]+=off_y[z];
            }
        rect_result.X=rect_soulth.X+off_x[0]/j;
        rect_result.Y=rect_soulth.Y+off_y[0]/j;
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

    Matrix gray_matrix_in;
    Matrix gray_matrix_in2;
    IOpnm::ReadPGMFile("img1.pgm", gray_matrix_in);
    IOpnm::ReadPGMFile("img2.pgm", gray_matrix_in2);


    RectangleS rect(150,90,50,30);
    RectangleS rect_result(100,100,300,300);

    if(track_surf(gray_matrix_in,  rect, gray_matrix_in2,rect_result))
    {
        itr_vision::Draw::Rectangle(gray_matrix_in, rect, 255);
        itr_vision::Draw::Rectangle(gray_matrix_in2, rect_result, 255);

        itr_vision::IOpnm::WritePGMFile("trackout1.pgm", gray_matrix_in);
        itr_vision::IOpnm::WritePGMFile("trackout2.pgm", gray_matrix_in2);
    }
    else
    {
        printf("\nerror! not matched!\n");
    }
    return 0;
}
