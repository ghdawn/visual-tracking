#ifndef GIMBAL_H
#define GIMBAL_H
#include "itrbase.h"
using itr_math::Vector;

namespace itr_tracker
{
    /**
    * \brief 提供云台控制的功能
    * \note 需要串口支持
    */
    class Gimbal
    {
        public:
            /**
            * \brief 初始化控制参数
            * \param degX 水平方向视场夹角(单位:角度)
            * \param degY 垂直方向向视场夹角(单位:角度)
            * \param u0 水平半幅图像的像素数(单位:像素)
            * \param v0 垂直半幅图像的像素数(单位:像素)
            */
            void Init(F32 degX,F32 degY,F32 u0,F32 v0);

            /**
            * \brief 利用跟踪结果控制云台
            * \param 向量X为系统的状态向量
            * \param X[0] x方向位置
            * \param X[1] y方向位置
            * \param X[2] x方向速度
            * \param X[3] y方向速度
            */
            void Control(Vector X);

            /**
            * \brief 直接控制云台转速
            * \param omegax x方向角速率
            * \param omegay y方向角速率
            */
            void Control(F32 omegax,F32 omegay);
        private:

    };
}
#endif // GIMBAL_H
