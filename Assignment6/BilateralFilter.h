//
// Created by XMacBook on 2022/12/5.
//

#ifndef DIP_BILATERALFILTER_H
#define DIP_BILATERALFILTER_H
#include "bmp.h"

class BilateralFilter: public BMP{
public:
    BilateralFilter(double sigmaS, double simgaT);
    PHOTO* bf(PHOTO *raw);
protected:
    double sigmaS;
    double sigmaT;
    PHOTO *inputPhoto;
    PHOTO *outputPhoto;
    double distanceSquareSpace(int h0, int w0, int h1, int w1);
    double distanceSquareColor(int h0, int w0, int h1, int w1);
    std::tuple<double,double,double> calcBFvalue(int h0, int w0);
};

#endif //DIP_BILATERALFILTER_H
