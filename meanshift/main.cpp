#include <iostream>
#include <cstdio>
#include "itrbase.h"
#include "itrvision.h"
#include "MeanShift.h"

using namespace itr_math;
using namespace std;


int main() {
    itr_math::MathObjStandInit();
    char filename[50];
    MeanShift meanShift;
    Matrix img1;
    itr_vision::IOpnm::ReadPGMFile("images/frame00000.pgm", img1);
    RectangleF rectf;
    rectf.X = 334;
    rectf.Y = 255;
    rectf.Width = 50;
    rectf.Height = 40;
    RectangleS rects(rectf.X, rectf.Y, rectf.Width, rectf.Height);
    meanShift.Init(img1, rectf,MeanShift::IMG_GRAY);
    for (int i = 1; i < 200; ++i) {
        Matrix img2;
        sprintf(filename,"images/frame%05d.pgm",i);
        itr_vision::IOpnm::ReadPGMFile(filename,img2);
        itr_vision::Draw::Rectangle(img1, rects, 255);
        meanShift.Go(img2, rectf);
        rects.X = rectf.X;
        rects.Y = rectf.Y;
        itr_vision::Draw::Rectangle(img2, rects, 255);
        sprintf(filename,"%d.pgm",i);
        itr_vision::IOpnm::WritePGMFile(filename, img2);

    }

}
