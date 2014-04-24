#include <iostream>
#include "itrvision.h"
#include "itralgorithm.h"

#define TRUST_MAX 50                //信任最大值，初始模板点置信度
#define TRUST_DEC       1           //模板中的点未能匹配上时 减小的置信度
#define TRUST_DEC_out   2           //与模板匹配的点跑出原有计算所得矩形区域时减小的置信度
#define TRUST_NEW 40                //新矩形区域出现新点的置信度
#define TRUST_DELETE -50             //置信度小于该值时，删除对应模板点

#define DELETE_VALUE 10                //坐标值误差超出该范围时，认为是错误，（SURF）像素坐标精度决定
#define ERR_VALUE 25                //计算结果误差超出该范围 认为计算错误，采用上次结果加 修正值 来代替
#define REPAIRE 2                   //修正值 （SURF）像素坐标精度决定
using namespace std;
using namespace itr_vision;


S32 center_cal(F32 *data,S32 length)
{
    F32 result;
    S32 Order;
    itr_math::CalculateObj->Max(data,length,result,Order);
    return(Order);
}

S32 track( std::vector<VectorFeaturePoint> &Modellist,///已知条件
           itr_math::RectangleS rect_source,///已知条件
           Matrix &mat2,///待匹配图片
           itr_math::RectangleS &rect_result,
           S32 _dx,S32 _dy)///返回结果
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

    S32 size_model =Modellist.size();
    S32 size_feature =FeaturePointList2.size();
    /// feature match
    FeatureMatch feature_matchobj;
    S32 matchnum=0;
    feature_matchobj.FeatureMatchDo( Modellist, FeaturePointList2,  matchnum);

    if(matchnum>0)
    {
        F32 *trust=new F32[matchnum]();
        F32 *Dx=new F32[matchnum]();
        F32 *Dy=new F32[matchnum]();
        S32 *id=new S32[matchnum]();
        S32 j=0;
        for(S32 i=0; i<size_model; i++)
        {
            if(Modellist[i].ID!=-1)
            {
                trust[j]=*((F32*)(Modellist[i].Tag));
                Dx[j]=FeaturePointList2[Modellist[i].ID].X-Modellist[i].X;
                Dy[j]=FeaturePointList2[Modellist[i].ID].Y-Modellist[i].Y;
                id[j]=i;
                j++;
            }
        }
        j=center_cal(trust,matchnum);
        F32 dx=Dx[j];
        F32 dy=Dy[j];
        F32 mdx,mdy;
        itr_math::StatisticsObj->Median(Dx,matchnum,mdx);
        itr_math::StatisticsObj->Median(Dy,matchnum,mdy);
        if(fabs(dx-mdx)>DELETE_VALUE||fabs(dy-mdy)>DELETE_VALUE)
        {
            dx=mdx;
            dy=mdy;
        }
        if(fabs(dx-_dx)>ERR_VALUE||fabs(dy-_dy)>ERR_VALUE)
        {
            if(dx>_dx)
                dx=_dx+REPAIRE;
            else
                dx=_dx-REPAIRE;
            if(dy>_dy)
                dy=_dy+REPAIRE;
            else
                dy=_dy-REPAIRE;
        }
        j=0;
        for(S32 i=0; i<matchnum; i++)
        {
            if(fabs(Dx[i]-dx)>DELETE_VALUE||fabs(Dy[i]-dy)>DELETE_VALUE)
            {
                Modellist.erase(Modellist.begin()+id[i]-j);
                j++;
                size_model--;
            }
        }


        rect_result.Height=rect_source.Height;
        rect_result.Width=rect_source.Width;
        rect_result.X=rect_source.X+dx;
        rect_result.Y=rect_source.Y+dy;
        itr_vision::Draw::Rectangle(mat2, rect_result, 255);

        itr_math::RectangleS rec_demo;
        rec_demo.Width=rect_result.Width;
        rec_demo.Height=rect_result.Height;
        rec_demo.X=rect_result.X;
        rec_demo.Y=rect_result.Y;

        for(S32 i=0; i<size_model; i++)
        {
            if(Modellist[i].ID==-1)
            {
                *((F32*)(Modellist[i].Tag)) -=TRUST_DEC;
            }
            else
            {
                if(rect_result.IsInRectangle(FeaturePointList2[Modellist[i].ID].X,FeaturePointList2[Modellist[i].ID].Y))
                {
                    if(rec_demo.IsInRectangle(FeaturePointList2[Modellist[i].ID].X,FeaturePointList2[Modellist[i].ID].Y))
                    {
                        *((F32*)(Modellist[i].Tag)) +=2*TRUST_DEC;
                    }
                    else
                    {
                        *((F32*)(Modellist[i].Tag)) +=TRUST_DEC;
                    }
                    FeaturePointList2[Modellist[i].ID].X -=dx;
                    FeaturePointList2[Modellist[i].ID].Y -=dy;
                    FeaturePointList2[Modellist[i].ID].ID=-2;                ///已经匹配上的不再添加进模板。
                }
                else
                {
                    *((F32*)(Modellist[i].Tag)) -=TRUST_DEC_out;
                    if(*((F32*)(Modellist[i].Tag))<TRUST_DELETE)
                    {
                        Modellist.erase(Modellist.begin()+i);
                        size_model--;
                        i--;
                    }
                }
            }
        }

        ///增加新点
        if(fabs(dx-_dx)<rect_result.Width*0.4&&fabs(dy-_dy)<rect_result.Width*0.4)
        {
            for(S32 i=0; i<size_feature; i++)
            {
                if(FeaturePointList2[i].ID!=-2)
                {
                    if(rec_demo.IsInRectangle(FeaturePointList2[i].X,FeaturePointList2[i].Y))
                    {
                        FeaturePointList2[i].Tag=new F32[1]();
                        *((F32*)(FeaturePointList2[i].Tag))=TRUST_NEW;
                        FeaturePointList2[i].X -=dx;
                        FeaturePointList2[i].Y -=dy;
                        FeaturePointList2[i].ID=-3;
                        Modellist.push_back(FeaturePointList2[i]);
                    }
                }
            }
            if(matchnum>20)
            {
                size_model=Modellist.size();
                for(S32 i=10; i>0; i--)
                {
                    S32 j=0;
                    while(Modellist[j].ID!=-1)
                    {
                        j++;
                        if(j==size_model)
                            break;
                    }
                    if(j==size_model)
                        break;
                    Modellist.erase(Modellist.begin()+j);
                    size_model--;
                }
            }
        }
        delete[] trust;
        delete[] Dx;
        delete[] Dy;
        delete[] id;
        ///debug
        printf("matched number:%d\t",matchnum);
        printf("rect offset:dX=%.2f\tdY=%.2f\n",dx,dy);
        return 1;
    }
    else
    {
        return 0;
    }
}


int last_main()
{
    itr_math::MathObjStandInit();

    char path[50]="bin/Debug/01_david/pgm/%05d.pgm";
    char file[50]="bin/Debug/01_david/pgm/00201.pgm";
    FILE* fout=fopen("bin/Debug/result.txt","w");
    Matrix pic_current,pic_last;
    IOpnm::ReadPGMFile(file, pic_last);

    ///计算模板图片特征点
    SURF surf;

    S32 w=pic_last.GetCol();
    S32 h=pic_last.GetRow();
    pic_last.AllMul(1/255.f);

    surf.Init(w,h,5,4,2,0.0002f);
    std::vector<VectorFeaturePoint> FeaturePointList;
    surf.ProcessAll(pic_last, FeaturePointList);
    S32 feature_num=FeaturePointList.size();

    pic_last.AllMul(255);

    /// 输入原始感兴趣区域
    RectangleS rect(0, 0, 0, 0);
    FILE *InitInfo=fopen("bin/Debug/01_david/init.txt","r");
    fscanf(InitInfo,"%d,%d,%d,%d",&rect.X,&rect.Y,&rect.Width,&rect.Height);
    fclose(InitInfo);
    rect.Width-=rect.X;
    rect.Height-=rect.Y;

    ///筛选感兴趣区域点
    std::vector<VectorFeaturePoint> Modellist;
    for(S32 i=0; i<feature_num; i++)
    {
        if(rect.IsInRectangle(FeaturePointList[i].X,FeaturePointList[i].Y))
        {
            FeaturePointList[i].Tag=new F32[1]();
            *((F32*)(FeaturePointList[i].Tag))=TRUST_NEW;
            Modellist.push_back(FeaturePointList[i]);
        }
    }

    RectangleS rect_result(0,0,0,0);
    RectangleS rectpoint(0,0,5,5);
    S32 _dx=0,_dy=0;
    //循环
    for(S32 i=202; i<763; i++)
    {
        sprintf(file, path, i);
        IOpnm::ReadPGMFile(file, pic_current);

        /// debuge
        printf("%d\n",i);
        /// ///////////////////////

        if(track(Modellist, rect,pic_current,rect_result,_dx,_dy))
        {
            _dx=rect_result.X-rect.X;
            _dy=rect_result.Y-rect.Y;

            /// show result in pictures/////////////////////////////
            itr_vision::Draw::Rectangle(pic_current,rect_result,255);
            sprintf(file,"bin/Debug/output/%05d.pgm",i);
            itr_vision::IOpnm::WritePGMFile(file,pic_current);

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

