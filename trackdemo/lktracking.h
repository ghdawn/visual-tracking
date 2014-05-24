#ifndef LKTRACKING_H
#define LKTRACKING_H

#include "itrbase.h"
#include "itralgorithm.h"
#include "itrvision.h"
#include "algorithm"
using namespace itr_algorithm;
using namespace itr_vision;
using itr_math::Matrix;
using itr_math::Point2D;
class lktracking
{
    public:
        /** Default constructor */
        lktracking();
        void Init(const Matrix &current,RectangleF &rect);
        bool Go(const Matrix &current,RectangleF &rect,F32 &Vx,F32 &Vy);
        /** Default destructor */
        virtual ~lktracking();
        S32 debugcount;
    protected:
    private:
    F32 getScale(S32 count);
    bool InSide(const Point2D& point,const RectangleF& rect) const;
    void Debug_Info();
    int fb_filter();
    void pairdistance(const vector<Point2D>& feature,vector<F32>& dist);
    int ncc_filter(const Matrix  &input1,const Matrix  &input2);

template <class T>
class DataOper:public Ransac<T,T>::Operator
{
    public:
        F32 GetError(T a, T b)
        {
            return (1+a-b)*(1+a-b);
        }
        T GetValue(T *data, S32 N)
        {
            std::sort(data,data+N);
            return data[N/2];
        }
        bool Remain(T a,T b)
        {
            return (fabs(a-b)<1.2);
        }
};
    S32 FeatureNum;
    vector<CommFeaturePoint> frame1Feature;
    vector<CommFeaturePoint> frame2Feature;
    vector<CommFeaturePoint> fbFeature;
    DataOper<F32> oper;
    Ransac<F32,F32> ransac;
    ConvoluteSquare conv;
    S32 trackedPoints;
    F32 *x,*y,*indexNo,*dist;
    LKTracker tracker;
    SelectKLTFeature* _select_pointer;
};

#endif // LKTRACKING_H
