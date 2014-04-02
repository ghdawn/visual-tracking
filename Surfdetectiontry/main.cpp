#include <iostream>
#include "itrvision.h"

#define pi2ang 1 //(180.0/3.1415926)
#define ang2pi 1 //(3.1415926/180.0)
#define min_dis_angle 0.2

#define trust_max 100
#define trust_dec 5
#define trust_dec_out 5
#define trust_new 49
#define trust_min 50
#define trust_clear -10000

using namespace std;
using namespace itr_vision;

void cof(VectorFeaturePoint pos1,F32* angle1,VectorFeaturePoint pos2,F32* angle2,F32* re_center)
{
    if(pos1.Dir*pi2ang-*angle1==pos2.Dir*pi2ang-*angle2)
    {
        *re_center=0;
        *(re_center+1)=0;
    }
    else
    {
        F32 tana1,tana2,cota1,cota2;
        itr_math::NumericalObj->Tan(pos1.Dir*pi2ang-*angle1,tana1);
        itr_math::NumericalObj->Tan(pos2.Dir*pi2ang-*angle2,tana2);
        /// 暂未考虑奇异
        /// debug
        F32 main_dir1=(pos1.Dir*pi2ang);
        F32 main_dir2=pos2.Dir*pi2ang;
        F32 angl_1=*angle1;
        F32 angl_2=*angle2;
//        cota1=1/tana1;
//        cota2=1/tana2;
        *re_center=(pos1.Y-pos2.Y+pos2.X*tana2-pos1.X*tana1)/(tana2-tana1);
        *(re_center+1)=(pos1.X-pos2.X+pos2.Y/tana2-pos1.Y/tana1)/(1/tana2-1/tana1);
    }
}

F32 _getangle(VectorFeaturePoint vec,F32 center_x,F32 center_y)
{
    F32 result;
    if(center_x-vec.X==0)
    {
        if(center_y<vec.Y)
            return (vec.Dir*pi2ang+PI/2);
        else
            return (vec.Dir*pi2ang-PI/2);
    }
    else
    {
        itr_math::NumericalObj->Atan2((F32)(center_y-vec.Y),(F32)(center_x-vec.X),result);
        if(result>0)
        {
            if(vec.Y>center_y)
                result=-PI+result;
        }
        else
        {
            if(result<0)
            {
                if(vec.Y<center_y)
                    result=PI+result;
            }
            else
            {
                if(vec.X>center_x)
                    result=PI;
            }
        }
        return((vec.Dir-result)*pi2ang);
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
    S32 *tmp_model=new S32[matched_num]();

    S32 size_of_model=Modellist.size();
    S32 size_p=FeaturePointList2.size();
    S32 cal_size=0;
    S32 model_match_index=0;

for(S32 i=0; i<size_of_model; i++)
{
    if((Modellist[i].ID!=-1&&*((F32*)(Modellist[model_match_index].Tag))>=trust_min))
    {
        tmp_model[cal_size]=i;
        cal_size++;
    }
}
printf("cal_size:%d\t",cal_size);
    if(cal_size>2)
    {
        do          /// RANSAC begin:
        {
            //pick 2 points
            for(S32 i=0; i<2; i++)
            {
                itr_math::NumericalObj->Rand(p);
                itr_math::NumericalObj->Floor(p*cal_size,q);
                if(q==cal_size)
                {
                    q=-1;
                    i--;
                    continue;
                }
                if(i>0)
                {
                    for(S32 z=0; z<i; z++)
                        if(q==lable[z])
                        {
                            q=-1;
                            i--;
                            continue;
                        }
                }
                if(q!=-1)
                    lable[i]=q;
            }

            cof(FeaturePointList2[Modellist[tmp_model[lable[0]]].ID], (F32*)(Modellist[tmp_model[lable[0]]].Tag)+1,
                FeaturePointList2[Modellist[tmp_model[lable[1]]].ID], (F32*)(Modellist[tmp_model[lable[1]]].Tag)+1,
                norm_center);
            if(j==0)
            {
                best_center[0]=norm_center[0];
                best_center[1]=norm_center[1];
                min_dis=1000;
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
                    dis+=fabs(getdismis(FeaturePointList2[Modellist[i].ID],((F32*)(Modellist[i].Tag) )+1,norm_center));
                }
                if((dis)<(min_dis))
                {
                    min_dis=dis;
                    best_center[0]=norm_center[0];
                    best_center[1]=norm_center[1];
                }
            }
            j++;
        }
        while(j<10);

        rect_result.X=best_center[0]-rect_soulth.Width/2;
        rect_result.Y=best_center[1]-rect_soulth.Height/2;
    }
    else
    {
        if(cal_size>0)
        {
            F32*dX=new F32[cal_size]();
            F32*dY=new F32[cal_size]();
            for(S32 i=0; i<cal_size; i++)
            {
                dX[i]=FeaturePointList2[Modellist[tmp_model[i]].ID].X-Modellist[tmp_model[i]].X;
                dY[i]=FeaturePointList2[Modellist[tmp_model[i]].ID].Y-Modellist[tmp_model[i]].Y;
            }
            F32 tmp_F32;
            itr_math::StatisticsObj->Median(dX,cal_size,tmp_F32);
            rect_result.X =rect_soulth.X+tmp_F32;
            itr_math::StatisticsObj->Median(dY,cal_size,tmp_F32);
            rect_result.Y =rect_soulth.Y+tmp_F32;
        }
        else
        {

            printf("\nno valid point!###########################\n");
            rect_result.X=rect_soulth.X;
            rect_result.Y=rect_soulth.Y;
        }
    }
    rect_result.Height=rect_soulth.Height;
    rect_result.Width=rect_soulth.Width;

    ///debug
    F32 dx=rect_result.X-rect_soulth.X;
    F32 dy=rect_result.Y-rect_soulth.Y;
    if(fabs(dx)>100||fabs(dy)>100)
    {
        printf("fatal errer!***********************************\n");
//        rect_result.X=rect_soulth.X;
//        rect_result.Y=rect_soulth.Y;
    }

    ///更新模板

    for(S32 i=0; i<size_of_model; i++)
    {
        if(!rect_result.IsInRectangle(Modellist[i].X,Modellist[i].Y))
        {
            F32* tmp_F32=(F32*)(Modellist[i].Tag);
            tmp_F32[0] -= trust_dec_out;
            if(tmp_F32[0]<trust_clear)
            {
                Modellist.erase(Modellist.begin()+i);
                size_of_model--;
                i--;
                continue;
            }
        }
        if(Modellist[i].ID==-1)
        {
            F32* tmp_F32=(F32*)(Modellist[i].Tag);
            tmp_F32[0] -= trust_dec;
            if(tmp_F32[0]<trust_clear)
            {
                Modellist.erase(Modellist.begin()+i);
                size_of_model--;
                i--;
            }
        }
        else
        {
            Modellist[i].X=FeaturePointList2[Modellist[i].ID].X;
            Modellist[i].Y=FeaturePointList2[Modellist[i].ID].Y;

            Modellist[i].Dir=FeaturePointList2[Modellist[i].ID].Dir;
            F32 *tmp_F32=(F32*)(Modellist[i].Tag);
            tmp_F32[1]=_getangle(FeaturePointList2[Modellist[i].ID],best_center[0],best_center[1]);
            ///匹配上的模板特征点增加信任度
            tmp_F32[0] +=trust_dec;
            if(tmp_F32[0]>trust_max)
                tmp_F32[0]=trust_max;
            FeaturePointList2[Modellist[i].ID].ID=1;///已经匹配上的不再添加进模板。
        }
    }

    ///增加新点

    for(S32 i=0; i<size_p; i++)
    {
        if(rect_result.IsInRectangle(FeaturePointList2[i].X,FeaturePointList2[i].Y)&&FeaturePointList2[i].ID!=1)
        {
            FeaturePointList2[i].ID=-2;
            FeaturePointList2[i].Tag=new F32[2]();
            F32* tmp_F32=(F32*)FeaturePointList2[i].Tag;
            tmp_F32[0]=trust_new;
            tmp_F32[1]=_getangle(FeaturePointList2[i], rect_result.X+rect_result.Width/2, rect_result.Y+rect_result.Height/2);

            if(getdismis(FeaturePointList2[i],((F32*)(FeaturePointList2[i].Tag) )+1,best_center) >min_dis_angle)
            {
                continue;
            }
            Modellist.push_back(FeaturePointList2[i]);
        }
    }
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
    mat2.AllMul(255.0);

    S32 size_feature =FeaturePointList2.size();
    S32 size_modl =Modellist.size();
    /// feature match
    FeatureMatch feature_matchobj;
    S32 matchnum=0;
    feature_matchobj.FeatureMatchDo( Modellist, FeaturePointList2,  matchnum);

    ///debuge draw////////////////////////////////////////////////
//    std::vector<itr_math::Point2D> pos1(matchnum);
//    std::vector<itr_math::Point2D> pos2(matchnum);
//    for(S32 m=0; m<size_modl; m++)
//    {
//    ;
//        if(Modellist[m].ID!=-1)//&&Modellist[m].Quality
//        {
//            pos2[m].X=FeaturePointList2[Modellist[m].ID].X;
//            pos2[m].Y=FeaturePointList2[Modellist[m].ID].Y;
//            pos1[m].X=Modellist[m].X;
//            pos1[m].Y=Modellist[m].Y;
//        }
//    }

//    Draw::Correspond(mat1,mat2,pos1,pos2,matchnum,reMat);
    ///debuge draw////////////////////////////////////////////////

    if(matchnum>0)
    {
        _centercal(Modellist,FeaturePointList2,matchnum,rect_soulth,rect_result);

        itr_vision::Draw::Rectangle(mat2, rect_result, 255);

        ///debug
        printf("matched number:%d\t",matchnum);
        F32 dx=rect_result.X-rect_soulth.X;
        F32 dy=rect_result.Y-rect_soulth.Y;

        printf("rect offset:dX=%.2f\tdY=%.2f\n",dx,dy);
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
    char file[50]="bin/Debug/01_david/pgm/00201.pgm";
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
            (FeaturePointList[i].Tag)=new F32[2]();
            F32* tmpf=(F32*)(FeaturePointList[i].Tag);
            *tmpf=trust_max;
            *(tmpf+1)=_getangle(FeaturePointList[i],rect.X+rect.Width/2,rect.Y+rect.Height/2);
            Modellist.push_back(FeaturePointList[i]);
        }
    }
/// debug
    S32 size_ofm=Modellist.size();
    RectangleS rectpoint(0, 0, 5, 5);
    for(S32 i=0; i<size_ofm; i++)
    {
        rectpoint.X=Modellist[i].X;
        rectpoint.Y=Modellist[i].Y;
        itr_vision::Draw::Rectangle(last,rectpoint,255);
    }
    itr_vision::Draw::Rectangle(last,rect,255);
    sprintf(file,"bin/Debug/output/model.pgm");
    itr_vision::IOpnm::WritePGMFile(file,last);

    RectangleS rect_result(0,0,0,0);
    //循环
    for(S32 i=202; i<762; i++)
    {
        sprintf(file, path, i);
        IOpnm::ReadPGMFile(file, current);
        /// debuge
        Matrix reMat;
        ///
        if(i==434)
        {
            i=434;
        }
        if(i==350)
        {
            i=350;
        }
        printf("%d\n",i);
        if(track_surf(Modellist, rect, last,current,reMat,rect_result))
        {
            last=current;
            itr_vision::Draw::Rectangle(current,rect_result,255);

            for(S32 i=0; i<Modellist.size(); i++)
            {
                if(Modellist[i].ID==-2)
                {
                    Modellist[i].ID=-1;
                    rectpoint.X=Modellist[i].X;
                    rectpoint.Y=Modellist[i].Y;
                    itr_vision::Draw::Circle(current,rectpoint.X,rectpoint.Y,3,255);
                }
                else
                {
                    if(Modellist[i].ID==-1)
                    {
                        rectpoint.X=Modellist[i].X;
                        rectpoint.Y=Modellist[i].Y;
                        itr_vision::Draw::Rectangle(current,rectpoint,0);
                    }
                    else
                    {
                        rectpoint.X=Modellist[i].X;
                        rectpoint.Y=Modellist[i].Y;
                        itr_vision::Draw::Rectangle(current,rectpoint,255);
                    }
                }
            }
            sprintf(file,"bin/Debug/output/%05d.pgm",i);
            itr_vision::IOpnm::WritePGMFile(file,current);
            ///debuge
//            sprintf(file,"bin/Debug/outmatch/%05dmatch.pgm",i);
//            itr_vision::IOpnm::WritePGMFile(file,reMat);
            ///

            fprintf(fout,"%d %d %d %d\n",rect_result.X,rect_result.Y,rect_result.X+rect_result.Width,rect_result.Y+rect_result.Height);
        }
        else
        {
            printf("\n error! not matched\n");
        }
    }
    fclose(fout);
    return 0;
}
