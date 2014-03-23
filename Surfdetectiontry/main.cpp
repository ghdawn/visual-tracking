#include <iostream>
#include "itrvision.h"

#define pi2ang (180.0/3.1415926)
#define ang2pi (3.1415926/180.0)

#define trust_max 100.0
#define trust_dec 20.0
#define trust_min 10.0
#define trust_new 20.0

#define max_resid 10.0
//#include "stdlib.h"
using namespace std;
using namespace itr_vision;

void cof(VectorFeaturePoint pos1,F32* angle1,VectorFeaturePoint pos2,F32* angle2,F32* re_center)
{
    if(pos1.Dir-*angle1==pos2.Dir-*angle2)
    {
        *re_center=0;
        *(re_center+1)=0;
    }
    else
    {
        F32 tana1,tana2,cota1,cota2;
        itr_math::NumericalObj->Tan(pos1.Dir-*angle1,tana1);
        itr_math::NumericalObj->Tan(pos2.Dir-*angle2,tana2);
        /// 暂未考虑奇异
//        cota1=1/tana1;
//        cota2=1/tana2;
        *re_center=(pos1.Y-pos2.Y+pos2.X*tana2-pos1.X*tana1)/(tana1-tana2);
        *(re_center+1)=(pos1.X-pos2.X+pos2.Y/tana2-pos1.Y/tana1)/(1/tana2-1/tana1);
    }
}

F32 _getangle(VectorFeaturePoint vec,F32 center_x,F32 center_y)
{   F32 result;
    if(center_x-vec.X==0)
        return (90.0-vec.Dir);
    else
    {
        itr_math::NumericalObj->Atan2((F32)(center_y-vec.Y),(F32)(center_x-vec.X),result);
        return(result*pi2ang-vec.Dir);
    }
}

F32 getdismis(VectorFeaturePoint pos1,F32* angle,F32* norm_center)
{
    return(*angle-_getangle(pos1,*norm_center,*(norm_center+1)));
}

void _centercal(     std::vector<VectorFeaturePoint> &Modellist,
                std::vector<VectorFeaturePoint> &FeaturePointList2,
                S32 matched_num,
                itr_math::RectangleS rect_soulth,
                itr_math::RectangleS &rect_result)
{

    S32 j=0;
    F32 p;
    S32 lable[2],q;
    F32 best_center[2];
    F32 norm_center[2];
    F32 min_dis,dis;
    do          /// RANSAC begin:
    {
        //pick 2 points
        for(S32 i=0; i<2; i++)
        {
            itr_math::NumericalObj->Rand(p);
            itr_math::NumericalObj->Floor(p*matched_num,q);
            if(q==matched_num)
            {
                itr_math::NumericalObj->Rand(p);
                itr_math::NumericalObj->Floor(p*matched_num,q);
            }
            if(i>0)
            {
                for(S32 z=0; z<i; z++)
                    if(q==lable[z])
                    {
                        q=-1;
                        i--;
                    }
            }
            if(Modellist[q].ID==-1)
            {
                q=-1;
                i--;
            }
            if(q!=-1)
                lable[i]=q;
        }

        cof(FeaturePointList2[Modellist[lable[0]].ID],(F32*)(Modellist[lable[0]].Tag)+1,
            FeaturePointList2[Modellist[lable[1]].ID],(F32*)(Modellist[lable[1]].Tag)+1,
            norm_center);
        if(j==0)
        {
            best_center[0]=norm_center[0];
            best_center[1]=norm_center[1];
            min_dis=100000000;
        }
        else
        {
            dis=0;
            for(S32 i=0; i<matched_num; i++)
            {
                while(Modellist[i].ID==-1)
                {
                    i++;
                }
                dis+=getdismis(FeaturePointList2[Modellist[i].ID],(F32*)(Modellist[i].Tag),norm_center);
            }
            if(dis<min_dis)
            {
                min_dis=dis;
                best_center[0]=norm_center[0];
                best_center[1]=norm_center[1];
            }
        }
        j++;
    }while(j<10);

//    norm_center[0]=best_center[0];
//    norm_center[1]=best_center[1];

    rect_result.Height=rect_soulth.Height;
    rect_result.Width=rect_soulth.Width;
    rect_result.X=best_center[0]-rect_result.Width/2;
    rect_result.Y=best_center[1]-rect_result.Height/2;

    ///剔除长时间不匹配的点
    S32 size_m=Modellist.size();
    for(S32 i=0; i<size_m; i++)
    {   if(Modellist[i].ID==-1)
        {
            *(F32*)(Modellist[i].Tag) -= trust_dec;
            if(*(F32*)(Modellist[i].Tag)<trust_min)
            {
                delete[] Modellist[i].Tag;
                Modellist.erase(Modellist.begin()+i);
                i--;
            }
//            FeaturePointList2[Modellist[i].ID].ID=0;
        }
        else
        {
            *(F32*)(Modellist[i].Tag)+=trust_dec;
            if(*(F32*)(Modellist[i].Tag)>trust_max)
                *(F32*)(Modellist[i].Tag)=trust_max;
        }
    }

    ///增加新点
     S32 size_p=FeaturePointList2.size();
    for(S32 i=0; i<size_p; i++)
    {   if(FeaturePointList2[i].ID!=1)
        {
            if(rect_result.IsInRectangle(FeaturePointList2[i].X,FeaturePointList2[i].Y))
            {
                FeaturePointList2[i].Tag=new F32[2]();
                *(F32*)(Modellist[i].Tag)=trust_new;
                *((F32*)(Modellist[i].Tag)+1)=_getangle(FeaturePointList2[i], rect_result.X+rect_result.Width/2, rect_result.Y+rect_result.Height/2);
                Modellist.push_back(FeaturePointList2[i]);
            }
        }
    }
//    ///精确拟合
//    F32 *resid_w=new F32[matched_num]();
//    F32 *resid=new F32[matched_num]();
//    F32 *dev=new F32[matched_num]();
//
//    for(S32 i=0; i<matched_num; i++)
//    {
//        resid_w[i]=1;
//    }
//    F32 sigma,median_value;
//        /// resid=x1;
//        /// dev=y1;
//    for(j=0; j<10; j++)
//    {
//
//        for(S32 i=0; i<matched_num; i++)
//        {
//            resid[i]=getdismis(FeaturePointList2[Modellist[i].ID],norm_center);
//        }
//        //计算权重
//
//        itr_math::StatisticsObj->Median(resid,matched_num,median_value);
//        sigma=1.4826*median_value;
//
//        for(S32 i=0; i<matched_num; i++)
//        {
//            dev[i]=resid[i]/sigma;
//            if(resid_w[i]>0)
//            {
//                itr_math::NumericalObj->Exp((-(dev[i]*dev[i])/2),resid_w[i]);
//                resid_w[i]/=sqrt(2*pi)*sigma;
//            }
//            if(resid[i]>max_resid)
//                resid_w[i]=0;
//        }
//        cof(x,y,z,resid_w,matched_num);
//        cof(FeaturePointList2[Modellist[lable[0]].ID],FeaturePointList2[Modellist[lable[1]].ID],norm_center);
//    }
}
S32 track_surf( std::vector<VectorFeaturePoint> &Modellist,///已知条件
                itr_math::RectangleS rect_soulth,///已知条件
                Matrix &mat1,///测试匹配用（debug）
                Matrix &mat2,///待匹配图片
                Matrix &reMat,///测试匹配用（debug）
                itr_math::RectangleS &rect_result)///返回结果
{
    /// calculate featurepoint of target picture
    SURF surf2;

    S32 w2=mat2.GetCol();
    S32 h2=mat2.GetRow();
    mat2.AllMul(1/255.f);

    surf2.Init(w2,h2,5,4,2,0.0002f);
    std::vector<VectorFeaturePoint> FeaturePointList2;
    surf2.ProcessAll(mat2, FeaturePointList2);
    S32 size_feature =FeaturePointList2.size();

    /// feature match
    FeatureMatch feature_matchobj;
    S32 matchnum=0;
    feature_matchobj.FeatureMatchDo( Modellist, FeaturePointList2,  matchnum);

    _centercal(Modellist,FeaturePointList2,matchnum,rect_soulth,rect_result);


    ///debuge
    std::vector<itr_math::Point2D> pos1(matchnum);
    std::vector<itr_math::Point2D> pos2(matchnum);
    for(S32 m=0; m<matchnum; m++)
    {
        if(Modellist[m].ID!=-1&&Modellist[m].Quality)
        {
            pos2[m].X=FeaturePointList2[Modellist[m].ID].X;
            pos2[m].Y=FeaturePointList2[Modellist[m].ID].Y;
            pos1[m].X=Modellist[m].X;
            pos1[m].Y=Modellist[m].Y;
            m++;
        }
    }
    mat2.AllMul(255);
    Draw::Correspond(mat1,mat2,pos1,pos2,matchnum,reMat);
    ///

    itr_vision::Draw::Rectangle(mat2, rect_result, 255);

//        mat1.AllMul(255);
//        itr_vision::IOpnm::WritePGMFile("trackout1.pgm", mat1);
//        itr_vision::IOpnm::WritePGMFile("trackout2.pgm", mat2);

    return 1;
}


int main()
{
    itr_math::MathObjStandInit();

    char path[50]="bin/Debug/01_david/pgm/%05d.pgm";
    char file[50]="bin/Debug/01_david/pgm/00298.pgm";
    FILE* fout=fopen("bin/Debug/result.txt","w");
    Matrix current,last;
    IOpnm::ReadPGMFile(file, last);

    ///计算模板图片特征点
    SURF surf;

    S32 w=last.GetCol();
    S32 h=last.GetRow();
    last.AllMul(1/255.f);

    surf.Init(w,h,5,4,2,0.0002f);
    std::vector<VectorFeaturePoint> FeaturePointList;
    surf.ProcessAll(last, FeaturePointList);
    S32 feature_num=FeaturePointList.size();

    last.AllMul(255);

    /// 读取原始感兴趣区域
    RectangleS rect(0, 0, 0, 0);
    FILE *InitInfo=fopen("bin/Debug/01_david/init.txt","r");
    fscanf(InitInfo,"%d,%d,%d,%d",&rect.X,&rect.Y,&rect.Width,&rect.Height);
    fclose(InitInfo);
    rect.Width-=rect.X;
    rect.Height-=rect.Y;

    ///筛选感兴趣区域点并初始计算模板方向和置信度
    std::vector<VectorFeaturePoint> Modellist;
    for(S32 i=0; i<feature_num; i++)
    {
        if(rect.IsInRectangle(FeaturePointList[i].X,FeaturePointList[i].Y))
        {
            FeaturePointList[i].Tag=new F32[2]();
            *((F32*)(Modellist[i].Tag))=trust_max;
            *((F32*)(Modellist[i].Tag)+1)=_getangle(FeaturePointList[i],rect.X+rect.Width/2,rect.Y+rect.Height/2);
            Modellist.push_back(FeaturePointList[i]);
        }
    }



    RectangleS rect_result(0,0,0,0);
    //循环
    for(S32 i=299; i<331; i++)
    {
        sprintf(file, path, i);
        IOpnm::ReadPGMFile(file, current);
        ///debuge
        if(i==320)
            i=320;
        /// debuge
        Matrix reMat;
        ///
        if(track_surf(Modellist, rect, last,current,reMat,rect_result))
        {
            itr_vision::Draw::Rectangle(current,rect_result,255);

            sprintf(file,"bin/Debug/output/%05d.pgm",i);
            itr_vision::IOpnm::WritePGMFile(file,current);
            ///debuge
            sprintf(file,"bin/Debug/output/%05dmatch.pgm",i);
            itr_vision::IOpnm::WritePGMFile(file,reMat);
            ///
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
