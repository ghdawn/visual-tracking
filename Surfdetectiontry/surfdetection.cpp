#include <iostream>
#include "itrvision.h"
#include "itralgorithm.h"

#define TRUST_MAX       50                //信任最大值，初始模板点置信度
#define TRUST_DEC       1           //模板中的点未能匹配上时 减小的置信度
#define TRUST_DEC_out   2           //与模板匹配的点跑出原有计算所得矩形区域时减小的置信度
#define TRUST_NEW       20                //新矩形区域出现新点的置信度
#define TRUST_DELETE    -50             //置信度小于该值时，删除对应模板点
#define TRUST_MIN    25

#define DELETE_VALUE    10                //坐标值误差超出该范围时，认为是错误，（SURF）像素坐标精度决定
#define ERR_VALUE       20                //计算结果误差超出该范围 认为计算错误，采用上次结果加 修正值 来代替
#define REPAIRE         2                   //修正值 （SURF）像素坐标精度决定

#define LOST_VALUE      1
using namespace std;
using namespace itr_vision;
S32 i_all;

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
           S32 _dx,S32 _dy,
           S32 &LostCount,bool IsLost,
           Matrix &matmodel)///返回结果
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

    S32 tmp_low;

    if(LostCount<LOST_VALUE)
        tmp_low=1;
    else
        tmp_low=4;
    if(matchnum>tmp_low)
    {
        F32 dx,dy;
        F32 *trust=new F32[matchnum]();
        F32 *Dx=new F32[matchnum]();
        F32 *Dy=new F32[matchnum]();
        S32 *id=new S32[matchnum]();
        F32 *d_rec_w=new F32[matchnum-1]();
        F32 *d_rec_h=new F32[matchnum-1]();

        S32 j=0,tmp_p=0,tmp_fm,valid_match=0,Mdc=0;
        for(S32 i=0; i<size_model; i++)
        {
            if(Modellist[i].ID!=-1&&*((F32*)(Modellist[i].Tag))>TRUST_MIN)
            {
                FeaturePointList2[Modellist[i].ID].ID=-2; //
                trust[j]=*((F32*)(Modellist[i].Tag));
                Dx[j]=FeaturePointList2[Modellist[i].ID].X-Modellist[i].X;
                Dy[j]=FeaturePointList2[Modellist[i].ID].Y-Modellist[i].Y;
                id[j]=i;

                if(tmp_p==0)
                {
                    tmp_p++;
                }
                else
                {
                    tmp_fm=(Modellist[i].X-Modellist[id[j-1]].X);
                    if(tmp_fm!=0)
                        d_rec_w[j-1]=(FeaturePointList2[Modellist[i].ID].X-FeaturePointList2[Modellist[id[j-1]].ID].X)-tmp_fm;
                    else
                        d_rec_w[j-1]=0;

                    tmp_fm=(Modellist[i].Y-Modellist[id[j-1]].Y);
                    if(tmp_fm!=0)
                        d_rec_h[j-1]=(FeaturePointList2[Modellist[i].ID].Y-FeaturePointList2[Modellist[id[j-1]].ID].Y)-tmp_fm;
                    else
                        d_rec_h[j-1]=0;

                    if(d_rec_h[j-1]>10)
                        d_rec_h[j-1]=10;

                    if(d_rec_h[j-1]<-10)
                        d_rec_h[j-1]=-10;

                    if(d_rec_w[j-1]>10)
                        d_rec_w[j-1]=10;

                    if(d_rec_w[j-1]<-10)
                        d_rec_w[j-1]=-10;
                }
                j++;
            }
//            else
//            {
//                if()
//                {
//                    if(matchnum>10)
//                    {
//                        Modellist.erase(Modellist.begin()+i);
//                        size_model--;
//                        i--;
//                    }
//                }
//            }
        }
        valid_match=j;
        if(valid_match<=0)
        {
            if(trust!=NULL)delete[] trust;
            if(Dx!=NULL)delete[] Dx;
            if(Dy!=NULL)delete[] Dy;
            if(id!=NULL)delete[] id;
            if(d_rec_w!=NULL)delete[] d_rec_w;
            if(d_rec_h!=NULL)delete[] d_rec_h;
//            trust=NULL;
//            Dx=NULL;
//            Dy=NULL;
//            d_rec_h=NULL;
//            id=NULL;
//            d_rec_w=NULL;
            LostCount=LOST_VALUE;
            return 0;
        }
        j=center_cal(trust,valid_match);
        dx=Dx[j];
        dy=Dy[j];
        F32 mdx,mdy;

        itr_math::StatisticsObj->Median(Dx,valid_match,mdx);
        itr_math::StatisticsObj->Median(Dy,valid_match,mdy);
        if(fabs(dx-mdx)>DELETE_VALUE||fabs(dy-mdy)>DELETE_VALUE)
        {
            dx=mdx;
            dy=mdy;
        }
        if(!IsLost)
            if(fabs(dx-_dx)>ERR_VALUE||fabs(dy-_dy)>ERR_VALUE)
            {
                LostCount++;
                {
                    if(trust!=NULL)delete[] trust;
                    if(Dx!=NULL)delete[] Dx;
                    if(Dy!=NULL)delete[] Dy;
                    if(id!=NULL)delete[] id;
                    if(d_rec_w!=NULL)delete[] d_rec_w;
                    if(d_rec_h!=NULL)delete[] d_rec_h;
                    trust=NULL;
                    Dx=NULL;
                    Dy=NULL;
                    d_rec_h=NULL;
                    id=NULL;
                    d_rec_w=NULL;
                    return 0;
                }
                if(dx>_dx)
                    dx=_dx+REPAIRE;
                else
                    dx=_dx-REPAIRE;
                if(dy>_dy)
                    dy=_dy+REPAIRE;
                else
                    dy=_dy-REPAIRE;
            }


       // j=0;
        S32 tmp_tar1,tmp_tar2;
        for(S32 i=0; i<valid_match; i++)
        {
            tmp_tar1=fabs(Dx[i]-dx)/2;
            tmp_tar2=fabs(Dy[i]-dy)/2;
            switch(tmp_tar1>tmp_tar2?tmp_tar1:tmp_tar2)
            {
            case 0:
                *((F32*)(Modellist[i].Tag)) +=10*TRUST_DEC;
                break;
            case 1:
                *((F32*)(Modellist[i].Tag)) +=5*TRUST_DEC;
                break;
            case 2:
                *((F32*)(Modellist[i].Tag)) +=TRUST_DEC;
                break;
            case 3:
                *((F32*)(Modellist[i].Tag)) -=TRUST_DEC;
                break;
            case 4:
                *((F32*)(Modellist[i].Tag)) -=5*TRUST_DEC;
                break;
//            case 5:
//                *((F32*)(Modellist[i].Tag)) -=10*TRUST_DEC;
//                break;
            default:
            {
                Modellist.erase(Modellist.begin()+id[i]-Mdc);
                Mdc++;
                size_model--;
            }
            }
        }

        ///scale
        if(valid_match>1)
        {
            itr_math::StatisticsObj->Median(d_rec_w,valid_match-1,mdx);
            itr_math::StatisticsObj->Median(d_rec_h,valid_match-1,mdy);
        }
        else
        {
            mdx=0;
            mdy=0;
        }

        rect_result.Height=rect_source.Height+mdy;
        rect_result.Width=rect_source.Width+mdx;
        rect_result.X=rect_source.X+dx-mdx/2;
        rect_result.Y=rect_source.Y+dy-mdy/2;
        itr_vision::Draw::Rectangle(mat2, rect_result, 255);

        itr_math::RectangleS rec_demo=rect_result;
        for(S32 i=0; i<size_model; ++i)
        {
            if(Modellist[i].ID==-1)
            {
                *((F32*)(Modellist[i].Tag)) -=5*TRUST_DEC;
            }
        }
/// ///////////////////////////////////////////////////////////
///特征点匹配关系输出
        Matrix cor;
        vector<Point2D> outU(valid_match),outV(valid_match);
        int count=0;

        for(S32 i=0; i<Modellist.size(); ++i)
        {
            if(Modellist[i].ID!=-1)
            {
                itr_math::Point2D p2d(Modellist[i].X,Modellist[i].Y);
                itr_math::Point2D p2dc(FeaturePointList2[Modellist[i].ID].X,FeaturePointList2[Modellist[i].ID].Y);
                outU[count]=p2d;
                outV[count]=p2dc;
                count++;
            }
        }
        Draw::Correspond(matmodel,mat2,outU,outV,count,cor);
        char file[20];
        sprintf(file,"bin/Debug/corr/corr%d",i_all);
        IOpnm::WritePGMFile(file,cor);


        ///增加新点
        if(fabs(dx-_dx)<rect_result.Width*0.4&&fabs(dy-_dy)<rect_result.Width*0.4)//rect_result.Width*0.4)
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
            //j=0;
                size_model=Modellist.size();
                for(S32 i=0; i<size_model; i++)
                {
                    if(*((F32*)(Modellist[i].Tag))<TRUST_DELETE)
                    {//++j;
                    Modellist.erase(Modellist.begin()+i);
                    size_model--;
                    --i;
                    }
                }
            }
        }

        if(trust!=NULL)delete[] trust;
        if(Dx!=NULL)delete[] Dx;
        if(Dy!=NULL)delete[] Dy;
        if(id!=NULL)delete[] id;
        if(d_rec_w!=NULL)delete[] d_rec_w;
        if(d_rec_h!=NULL)delete[] d_rec_h;
        trust=NULL;
        Dx=NULL;
        Dy=NULL;
        d_rec_h=NULL;
        id=NULL;
        d_rec_w=NULL;
        ///debug
        printf("vad number:%d\t",valid_match);
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

    char path[50]="bin/Debug/carchase/pgm/%05d.pgm";
    char file[50]="bin/Debug/carchase/pgm/00208.pgm";
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
    FILE *InitInfo=fopen("bin/Debug/carchase/init1.txt","r");
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
            *((F32*)(FeaturePointList[i].Tag))=TRUST_MAX;
            Modellist.push_back(FeaturePointList[i]);
        }
    }

    RectangleS rect_result(0,0,0,0);
    RectangleS rectpoint(0,0,5,5);
    S32 _dx=0,_dy=0;
    S32 LostCount=0;
    bool IsLost=false;
    //循环
    for( i_all=209; i_all<900; i_all++)
    {
        sprintf(file, path, i_all);
        IOpnm::ReadPGMFile(file, pic_current);
        if(i_all==355)
            i_all=355;
        if(i_all==388)
            i_all=388;
        /// debuge
        printf("%d\t",i_all);
        /// ///////////////////////

        if(track(Modellist, rect,pic_current,rect_result,_dx,_dy,LostCount,IsLost,pic_last))
        {
            LostCount=0,IsLost=false;

            if(LostCount<LOST_VALUE)
            {
                _dx=rect_result.X-rect.X;
                _dy=rect_result.Y-rect.Y;

                /// show result in pictures/////////////////////////////
                itr_vision::Draw::Rectangle(pic_current,rect_result,255);
                sprintf(file,"bin/Debug/output/%05d.pgm",i_all);
                itr_vision::IOpnm::WritePGMFile(file,pic_current);

                fprintf(fout,"%d: %d %d %d %d\n",i_all,rect_result.X,rect_result.Y,rect_result.X+rect_result.Width,rect_result.Y+rect_result.Height);
            }
            else
            {

                itr_vision::Draw::Rectangle(pic_current,rect,0);
                sprintf(file,"bin/Debug/output/%05d.pgm",i_all);
                itr_vision::IOpnm::WritePGMFile(file,pic_current);
            }
        }
        else
        {
            IsLost=true;
            if(LostCount>LOST_VALUE)LostCount=LOST_VALUE;
            printf(" not matched\n");
        }
    }
    fclose(fout);
    return 0;
}

