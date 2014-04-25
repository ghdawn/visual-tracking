#include <iostream>
#include "itrvision.h"
#include "surfdetection.h"

using namespace std;
using namespace itr_vision;

#ifdef f
#define min_dis_angle 0.2
#define min_angle 0.00005     //根据角度值计算中心点坐标时，所能识别的分辨率——针对tan(pi/2)和cot(0)
#define trust_max 100       //信任最大值，防止溢出的可能
#define trust_dec       5         //模板中的点未能匹配上时 减小的置信度
#define trust_dec_out   5     //模板上的点跑出原有矩形区域时减小的置信度。理论上不需要。。。。
#define trust_new 5         //新矩形区域出现新点的信任度
#define trust_min 20        //参与计算的最小置信度
#define trust_clear -10000


//#define sim

void cof(F32 pos1X,F32 pos1Y,F32 angle1,F32 pos2X,F32 pos2Y,F32 angle2,F32& re_center_x,F32& re_center_y)
{
    if((angle1-angle2)==0)
    {
        re_center_x=0;
        re_center_y=0;
    }
    else
    {
        F32 tana1,tana2;
        if(fabs(angle1-PI/2)<min_angle||fabs(angle1+PI/2)<min_angle)
        {
            re_center_x=pos1X;
            if(fabs(angle2)<min_angle)
                re_center_y=pos2Y;
            else
            {
                itr_math::NumericalObj->Tan(angle2,tana2);
                re_center_y=pos2Y+(pos1X-pos2X)/tana2;
            }
        }
        else
        {
            if(fabs(angle2-PI/2)<min_angle||fabs(angle2+PI/2)<min_angle)
            {
                re_center_x=pos2X;
                if(fabs(angle1)<min_angle)
                    re_center_y=pos1Y;
                else
                {
                    itr_math::NumericalObj->Tan(angle1,tana1);
                    re_center_y=pos1Y+(pos2X-pos1X)/tana1;
                }
            }
            else
            {
                itr_math::NumericalObj->Tan(angle1,tana1);
                itr_math::NumericalObj->Tan(angle2,tana2);
                re_center_x=(pos1Y-pos2Y+pos2X*tana2-pos1X*tana1)/(tana2-tana1);
                re_center_y=(pos1X-pos2X+pos2Y/tana2-pos1Y/tana1)/(1/tana2-1/tana1);
            }
        }
        if(re_center_x<1||re_center_x>320||re_center_y<1||re_center_y>240)
        {
            printf("cof errer!\n");
        }
    }
}
F32 getangle(F32 vecX,F32 vecY,F32 center_x,F32 center_y)
{
    F32 result;
    if(center_x-vecX==0)
    {
        if(center_y<vecY)
            return (PI/2);
        else
            return (-PI/2);
    }
    else
    {
        itr_math::NumericalObj->Atan2((F32)(center_y-vecY),(F32)(center_x-vecX),result);
        if(result>0)
        {
            if(vecY>center_y)
                result=-PI+result;
        }
        else
        {
            if(result<0)
            {
                if(vecY<center_y)
                    result=PI+result;
            }
            else
            {
                if(vecX>center_x)
                    result=PI;
            }
        }
        return(result);
    }
}

#ifdef sim
void sim_cof(F32 Model_pos1X,F32 Model_pos1Y,F32 Model_pos2X,F32 Model_pos2Y,
             F32 Tar_pos1X,F32 Tar_pos1Y,F32 Tar_pos2X,F32 Tar_pos2Y,
             F32 model_center_x,F32 model_center_y,
             F32& norm_center_x,F32& norm_center_y)
{
    F32 angle1,angle2;
    angle1=getangle(Model_pos1X,Model_pos1Y,model_center_x,model_center_y)-getangle(Model_pos1X,Model_pos1Y,Model_pos2X,Model_pos2Y)+getangle(Tar_pos1X,Tar_pos1Y,Tar_pos2X,Tar_pos2Y);
    angle2=getangle(Model_pos2X,Model_pos2Y,model_center_x,model_center_y)-getangle(Model_pos2X,Model_pos2Y,Model_pos1X,Model_pos1Y)+getangle(Tar_pos2X,Tar_pos2Y,Tar_pos1X,Tar_pos1Y);
    cof(Tar_pos1X,Tar_pos1Y,angle1,Tar_pos2X,Tar_pos2Y,angle2,norm_center_x,norm_center_y);

    ///debug
    if(fabs(norm_center_x-model_center_x)>50||fabs(norm_center_y-model_center_y)>50)
    {
        // printf("sim_cof errer!\n");
    }
}
F32 sim_getdis(F32 x1,F32 y1,itr_math::RectangleS rect)
{
    if(rect.IsInRectangle(x1,y1))
        return(sqrt((x1-rect.X)*(x1-rect.X)+(y1-rect.Y)*(y1-rect.Y)));
    else
        return(200);
}
#else
F32 dir_getdis(VectorFeaturePoint vec,itr_math::RectangleS rect)
{
    F32 *p_f32=(F32*)(vec.Tag);
    F32 tmp=fabs(getangle(vec.X,vec.Y,rect.X+rect.Width/2,rect.Y+rect.Height/2)-vec.Dir-p_f32[1]);
    while(tmp>PI)
    {
        tmp-=PI;
    }
    return(tmp);

}
F32 dir_getdis3(VectorFeaturePoint vec,itr_math::RectangleS rect)
{
    F32 *p_f32=(F32*)(vec.Tag);
    if(rect.IsInRectangle(vec.X,vec.Y))
    {
        F32 tmp=fabs(getangle(vec.X,vec.Y,rect.X+rect.Width/2,rect.Y+rect.Height/2)-vec.Dir-p_f32[1]);
        while(tmp>PI)
        {
            tmp-=PI;
        }
        return(tmp);
    }
    else
    {
        return(100);
    }
}
#endif

void centercal(     std::vector<VectorFeaturePoint> &Modellist,
                    std::vector<VectorFeaturePoint> &FeaturePointList2,
                    S32 matched_num,
                    itr_math::RectangleS rect_source,
                    itr_math::RectangleS &rect_result)
{
    rect_result.Height=rect_source.Height;
    rect_result.Width=rect_source.Width;

    S32 jj=0;
    F32 p;
    S32 lable[2],q;
    F32 best_center[2];
    F32 norm_center[2];
    F32 min_dis,dis,dis_c,min_disc;
    S32 *tmp_model=new S32[matched_num]();

    S32 size_of_model=Modellist.size();
    S32 size_p=FeaturePointList2.size();
    S32 cal_size=0;
    //S32 model_match_index=0;

    for(S32 i=0; i<size_of_model; i++)
    {
        if((Modellist[i].ID!=-1&&*((F32*)(Modellist[i].Tag))>=trust_min))
        {
            tmp_model[cal_size]=i;
            cal_size++;
        }
    }
    for(S32 i=0; i<size_p; i++)
    {
        FeaturePointList2[i].ID=-1;
    }
/// ///////////////////////////////
    printf("cal_size:%d\t",cal_size);

    if(cal_size>2)
    {
        S32 bucket_counter[9]= {0};
        S32 *bucket=new S32 [9*cal_size]();
        F32* tempvalue_u=new F32[cal_size]();
        F32* tempvalue_v=new F32[cal_size]();


        for(S32 i=0; i<cal_size; i++)
        {
            tempvalue_u[i]=Modellist[tmp_model[i]].X;
            tempvalue_v[i]=Modellist[tmp_model[i]].Y;
        }
        F32 maxu,maxv,minu,minv;
        itr_math::CalculateObj->Max(tempvalue_u,cal_size,maxu);
        itr_math::CalculateObj->Min(tempvalue_u,cal_size,minu);
        itr_math::CalculateObj->Max(tempvalue_v,cal_size,maxv);
        itr_math::CalculateObj->Min(tempvalue_v,cal_size,minv);
        F32 U3=minu*2/3+maxu/3;
        F32 V3=minv*2/3+maxv/3;
        F32 U23=minu/3+maxu/3*2;
        F32 V23=minv/3+maxv/3*2;

        /// bucket for 9 bucket
        for(S32 i=0; i<cal_size; i++)
        {
            if(tempvalue_u[i]<=U3&&tempvalue_v[i]<=V3)
            {
                *(bucket+(0*cal_size+bucket_counter[0]))=i;
                bucket_counter[0]++;
                continue;
            }
            else if(tempvalue_u[i]<=U3&&tempvalue_v[i]<=V23)
            {
                *(bucket+(1*cal_size+bucket_counter[1]))=i;
                bucket_counter[1]++;
                continue;
            }
            else if(tempvalue_u[i]<=U3)
            {
                *(bucket+(2*cal_size+bucket_counter[2]))=i;
                bucket_counter[2]++;
                continue;
            }

            else if(tempvalue_u[i]<=U23&&tempvalue_v[i]<=V3)
            {
                *(bucket+(3*cal_size+bucket_counter[3]))=i;
                bucket_counter[3]++;
                continue;
            }
            else if(tempvalue_u[i]<=U23&&tempvalue_v[i]<=V23)
            {
                *(bucket+(4*cal_size+bucket_counter[4]))=i;
                bucket_counter[4]++;
                continue;
            }
            else if(tempvalue_u[i]<=U23)
            {
                *(bucket+(5*cal_size+bucket_counter[5]))=i;
                bucket_counter[5]++;
                continue;
            }

            else if(tempvalue_v[i]<=V3)
            {
                *(bucket+(6*cal_size+bucket_counter[6]))=i;
                bucket_counter[6]++;
                continue;
            }
            else if(tempvalue_v[i]<=V23)
            {
                *(bucket+(7*cal_size+bucket_counter[7]))=i;
                bucket_counter[7]++;
                continue;
            }
            else
            {
                bucket_counter[8]++;
                *(bucket+(8*cal_size+bucket_counter[8]))=i;
                continue;
            }
        }

        S32 amptybucket=0;
        for(S32 i=0; i<9; i++)
            if(bucket_counter[i]==0)
                amptybucket++;
        if(amptybucket>7)
        {
            printf("no enough valid bucket!\n");

        }
        F32 ratio_bucket[9]= {0};
        ratio_bucket[0]=((F32)bucket_counter[0])/cal_size;
        for(S32 i=1; i<16; i++)
        {
            ratio_bucket[i]=ratio_bucket[i-1] + (F32)bucket_counter[i]/cal_size;
        }

        S16 b[2]= {0};
        S32 tmp_k;
        jj=0;
        do          /// RANSAC begin:
        {
            //pick 2 bucket
            for(S32 j=0; j<2; j++)
            {
                itr_math::NumericalObj->Rand(p);
                for(S32 k=0; k<9; k++)
                {
                    if(k==0)
                    {
                        if(p<ratio_bucket[0])
                        {
                            q=0;
                            break;
                        }
                    }
                    else
                    {
                        if(ratio_bucket[k-1]<p&&ratio_bucket[k]>=p)
                        {
                            if(bucket_counter[k]>0)
                                q=k;
                            else
                            {
                                tmp_k =k;
                                while(bucket_counter[tmp_k]==0)
                                    tmp_k--;
                                q=tmp_k;
                            }
                            break;
                        }
                    }
                }
                if(j>0)
                {
                    for(S32 z=0; z<j; z++)
                        if(q==b[z])
                        {
                            q=-1;
                            j--;
                        }
                }
                if(q!=-1)
                    b[j]=q;
            }
            //pick 2 points
            for(S32 j=0; j<2; j++)
            {
                itr_math::NumericalObj->Rand(p);
                itr_math::NumericalObj->Floor(p*bucket_counter[b[j]],q);
                lable[j]=tmp_model[*(bucket+(b[j]*cal_size+q))];
            }
#ifdef sim
            sim_cof(Modellist[lable[0]].X,Modellist[lable[0]].Y,Modellist[lable[1]].X,Modellist[lable[1]].Y,
                    FeaturePointList2[Modellist[lable[0]].ID].X,FeaturePointList2[Modellist[lable[0]].ID].Y,
                    FeaturePointList2[Modellist[lable[1]].ID].X,FeaturePointList2[Modellist[lable[1]].ID].Y,
                    rect_source.X+rect_source.Width/2,rect_source.Y+rect_source.Height/2,
                    norm_center[0],norm_center[1]);

#else
            F32* p_F32=(F32*)(Modellist[lable[0]].Tag);
            F32 angle1=FeaturePointList2[Modellist[lable[0]].ID].Dir-p_F32[1];
            p_F32=(F32*)(Modellist[lable[1]].Tag);
            F32 angle2=FeaturePointList2[Modellist[lable[1]].ID].Dir-p_F32[1];

            cof(FeaturePointList2[Modellist[lable[0]].ID].X,FeaturePointList2[Modellist[lable[0]].ID].Y,
                angle1,
                FeaturePointList2[Modellist[lable[1]].ID].X,FeaturePointList2[Modellist[lable[1]].ID].Y,
                angle2,
                norm_center[0],norm_center[1]);
#endif
            if(jj==0)
            {
                best_center[0]=norm_center[0];
                best_center[1]=norm_center[1];
                min_dis=1000000;
                min_disc=0;
            }
            else
            {
                dis=0;
                dis_c=0;
                rect_result.X=norm_center[0]-rect_source.Width/2;
                rect_result.Y=norm_center[1]-rect_source.Height/2;
#ifdef sim
                for(S32 i=0; i<size_of_model; i++)
                {
                    dis+=sim_getdis(Modellist[i].X,Modellist[i].Y,rect_result);
                }
#else
//                { //version 1
//                    for(S32 i=0; i<size_of_model; i++)
//                {
//                    dis+=dir_getdis(Modellist[i],rect_result);
//                }

//                }
//                {   //version 2
//                for(S32 i=0; i<size_of_model; i++)
//                {
//                    if(rect_result.IsInRectangle(Modellist[i].X,Modellist[i].Y))
//                    {
//                        dis_c++;
//                    }
//                }
//                dis=(rect_source.X-rect_result.X)*(rect_source.X-rect_result.X);
//                dis+=(rect_source.Y-rect_result.Y)*(rect_source.Y-rect_result.Y);
//                }
                {
                    //version 3
                    for(S32 i=0; i<size_of_model; i++)
                    {
                        if((Modellist[i].ID!=-1&&*((F32*)(Modellist[i].Tag))>=trust_min))
                        {
                           dis+=dir_getdis3(Modellist[i],rect_result);
                        }

                    }

                }
#endif
                if(dis_c>min_disc)
                {
                    min_dis=dis;
                    min_disc=dis_c;
                    best_center[0]=norm_center[0];
                    best_center[1]=norm_center[1];
                }
                else if(dis_c==min_disc&&(dis)<(min_dis))
                {
                    min_dis=dis;
                    best_center[0]=norm_center[0];
                    best_center[1]=norm_center[1];
                }
            }
            jj++;
        }
        while(jj<10);

        rect_result.X=best_center[0]-rect_source.Width/2;
        rect_result.Y=best_center[1]-rect_source.Height/2;

        delete[] bucket;
        delete[] tempvalue_u;
        delete[] tempvalue_v;
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
            rect_result.X =rect_source.X+tmp_F32;
            itr_math::StatisticsObj->Median(dY,cal_size,tmp_F32);
            rect_result.Y =rect_source.Y+tmp_F32;
        }
        else
        {
            printf("\nno valid point!###########################\n");
            S32 i=0;
            while(Modellist[i].ID==-1)
            {
                i++;
            }
            rect_result.X=rect_source.X+FeaturePointList2[Modellist[i].ID].X-Modellist[i].X;
            rect_result.Y=rect_source.Y+FeaturePointList2[Modellist[i].ID].Y-Modellist[i].Y;
        }
    }


    ///debug
    F32 dx=rect_result.X-rect_source.X;
    F32 dy=rect_result.Y-rect_source.Y;
    if(fabs(dx)>50||fabs(dy)>50)
    {
        printf("fatal errer!***********************************\n");
//        rect_result.X=rect_source.X;
//        rect_result.Y=rect_source.Y;
    }

    ///更新模板
    F32 tmp_c_x,tmp_c_y;
    tmp_c_x=best_center[0]-(rect_source.X+rect_source.Width/2);
    tmp_c_y=best_center[1]-(rect_source.Y+rect_source.Height/2);

    for(S32 i=0; i<size_of_model; i++)
    {
//        if(!rect_source.IsInRectangle(Modellist[i].X,Modellist[i].Y))
//        {
//            F32* tmp_F32=(F32*)(Modellist[i].Tag);
//            tmp_F32[0] -= trust_dec_out;
//            if(tmp_F32[0]<trust_clear)
//            {
//                Modellist.erase(Modellist.begin()+i);
//                size_of_model--;
//                i--;
//                continue;
//            }
//        }
        //F32 tmp_change_x=0,


        if(Modellist[i].ID==-1)     /// 未匹配上的 降低信任度
        {
            F32* tmp_F32=(F32*)(Modellist[i].Tag);
            tmp_F32[0] -= trust_dec;
#ifdef sim
            /// updata X Y;
            Modellist[i].X +=tmp_c_x;
            Modellist[i].Y +=tmp_c_y;
#else
//            Modellist[i].X +=tmp_c_x;
//            Modellist[i].Y +=tmp_c_y;
            //tmp_F32[1]=Modellist[i].Dir - getangle( Modellist[i].X, Modellist[i].Y,best_center[0],best_center[1]);
#endif
            if(tmp_F32[0]<trust_clear)
            {
                Modellist.erase(Modellist.begin()+i);
                size_of_model--;
                i--;
            }
        }
        else
        {
            ///匹配上的模板特征点增加信任度
            F32 *tmp_F32=(F32*)(Modellist[i].Tag);
            tmp_F32[0] +=trust_dec;
            if(tmp_F32[0]>trust_max)
                tmp_F32[0]=trust_max;
#ifdef sim
//            tmp_c_x=fabs( Modellist[i].X-FeaturePointList2[Modellist[i].ID].X);
//            tmp_change_x+=tmp_c_x;
//
//            tmp_c_y=fabs( Modellist[i].Y-FeaturePointList2[Modellist[i].ID].Y);
//            tmp_change_y+=tmp_c_y;
//
//            tmp_change_c++;
            if(fabs(Modellist[i].X-FeaturePointList2[Modellist[i].ID].X)<1.5*tmp_c_x&&
                    fabs( Modellist[i].Y=FeaturePointList2[Modellist[i].ID].Y)<1.5*tmp_c_y)
            {
                Modellist[i].X=FeaturePointList2[Modellist[i].ID].X;
                Modellist[i].Y=FeaturePointList2[Modellist[i].ID].Y;
            }
            else
            {
                Modellist[i].X +=tmp_c_x;
                Modellist[i].Y +=tmp_c_y;
            }
#else
//            if(rect_result.IsInRectangle(FeaturePointList2[Modellist[i].ID].X,FeaturePointList2[Modellist[i].ID].Y))
//            {
//                Modellist[i].X=FeaturePointList2[Modellist[i].ID].X;
//                Modellist[i].Y=FeaturePointList2[Modellist[i].ID].Y;
//            }
//            else
//            {
//                Modellist[i].X +=tmp_c_x;
//                Modellist[i].Y +=tmp_c_y;
//            }
            //Modellist[i].Dir=FeaturePointList2[Modellist[i].ID].Dir;
            tmp_F32[1]= Modellist[i].Dir - getangle( Modellist[i].X, Modellist[i].Y,best_center[0],best_center[1]);
#endif
            FeaturePointList2[Modellist[i].ID].ID=1;///已经匹配上的不再添加进模板。
        }
    }

    ///增加新点
/*
    for(S32 i=0; i<size_p; i++)
    {
        if(rect_result.IsInRectangle(FeaturePointList2[i].X,FeaturePointList2[i].Y)&&FeaturePointList2[i].ID!=1)
        {
            ///debug
            ///////////////////////////////////
            F32 tmp_f32=FeaturePointList2[i].X-best_center[0];
            if(fabs(tmp_f32)>60)
            {
                i=i;
                i=i;
            }

            FeaturePointList2[i].ID=-2;
#ifdef sim
            FeaturePointList2[i].Tag=new F32[1]();
            *((F32*)(FeaturePointList2[i].Tag))=trust_new;
#else
            (FeaturePointList2[i].Tag)=new F32[2]();
            F32* tmpf=(F32*)(FeaturePointList2[i].Tag);
            *tmpf=trust_new;
            *(tmpf+1)= FeaturePointList2[i].Dir-getangle(FeaturePointList2[i].X,FeaturePointList2[i].Y,best_center[0],best_center[1]);
#endif
            Modellist.push_back(FeaturePointList2[i]);
        }
    }*/
    delete[] tmp_model;
}

S32 track_surf( std::vector<VectorFeaturePoint> &Modellist,///已知条件
                itr_math::RectangleS rect_source,///已知条件
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

    //S32 size_feature =FeaturePointList2.size();
    S32 size_modl =Modellist.size();
    /// feature match
    FeatureMatch feature_matchobj;
    S32 matchnum=0;
    feature_matchobj.FeatureMatchDo( Modellist, FeaturePointList2,  matchnum);

    if(matchnum>0)
    {
        centercal(Modellist,FeaturePointList2,matchnum,rect_source,rect_result);

        itr_vision::Draw::Rectangle(mat2, rect_result, 255);

        ///debug
        printf("matched number:%d\t",matchnum);
        F32 dx=rect_result.X-rect_source.X;
        F32 dy=rect_result.Y-rect_source.Y;

        printf("rect offset:dX=%.2f\tdY=%.2f\n",dx,dy);
        return 1;
    }
    else
    {
        return 0;
    }
}
int f_main()
{
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
#ifdef sim
            FeaturePointList[i].Tag=new F32[1]();
            *((F32*)(FeaturePointList[i].Tag))=trust_max;
#else
            (FeaturePointList[i].Tag)=new F32[2]();
            F32* tmpf=(F32*)(FeaturePointList[i].Tag);
            *tmpf=trust_max;
            *(tmpf+1)= FeaturePointList[i].Dir-
                       getangle(FeaturePointList[i].X,FeaturePointList[i].Y,rect.X+rect.Width/2,rect.Y+rect.Height/2);
#endif
            Modellist.push_back(FeaturePointList[i]);
        }
    }


    RectangleS rect_result(0,0,0,0);
    //循环
    for(S32 i=202; i<762; i++)
    {
        sprintf(file, path, i);
        IOpnm::ReadPGMFile(file, current);
        /// debuge
        Matrix reMat;

        if(i==219)
        {
            i=219;
        }
        if(i==206)
        {
            i=206;
        }
        printf("%d\n",i);
        /// ///////////////////////

        if(track_surf(Modellist, rect, last,current,reMat,rect_result))
        {
            last=current;
            itr_vision::Draw::Rectangle(current,rect_result,255);

            for(S32 ii=0; ii<Modellist.size(); ii++)
            {
                if(*((F32*)(Modellist[ii].Tag))>trust_min)
                {
                    if(Modellist[ii].ID==-2)
                    {
                        Modellist[ii].ID=-1;
                        rectpoint.X=Modellist[ii].X;
                        rectpoint.Y=Modellist[ii].Y;
                        itr_vision::Draw::Circle(current,rectpoint.X,rectpoint.Y,3,255);
                    }
                    else
                    {
                        if(Modellist[ii].ID==-1)
                        {
                            rectpoint.X=Modellist[ii].X;
                            rectpoint.Y=Modellist[ii].Y;
                            itr_vision::Draw::Rectangle(current,rectpoint,0);
                        }
                        else
                        {
                            rectpoint.X=Modellist[ii].X;
                            rectpoint.Y=Modellist[ii].Y;

                            if(rectpoint.X-rect_result.X>rect_result.Width)
                            {
                                // printf("out!\n");
                            }

                            itr_vision::Draw::Rectangle(current,rectpoint,255);
                        }
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
#endif
int main()
{
    itr_math::MathObjStandInit();
    //fore
#ifdef f
    f_main();
#endif
    //later
    last_main();
}
