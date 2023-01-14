//
// Created by XMacBook on 2022/12/5.
//

#include "BilateralFilter.h"
#include <cmath>
#include <iostream>
#define GAUSS_CONST 0.39894228

BilateralFilter::BilateralFilter(double s, double t) {
    if (s <= 0 || t <= 0) {
        throw "sigma value must be above zero!";
    }
    sigmaS = s;
    sigmaT = t;
}


PHOTO *BilateralFilter::bf(PHOTO *raw) {
    if (!raw) {
        throw "[err] nullptr!";
    }
    std::cout << "[log] start bilateral filtering..." << std::endl;
    inputPhoto = raw;
    outputPhoto = imgnew(inputPhoto->width, inputPhoto->height);
    int h, w;
    for (h = 0; h < inputPhoto->height; h++) {
        for (w = 0; w < inputPhoto->width; w++) {
            auto [r,g,b] = calcBFvalue(h, w);
            // outputPhoto->img[h][w].rgbReserved = .0;
            outputPhoto->img[h][w].rgbRed = r;
            outputPhoto->img[h][w].rgbGreen = g;
            outputPhoto->img[h][w].rgbBlue = b;
        }
        if (!(h%10)) {
            std::cout << "[log] running...  " <<h << '/' << inputPhoto->height << std::endl;
        }

    }
    return outputPhoto;
}


std::tuple<double,double,double> BilateralFilter::calcBFvalue(const int h0, const int w0) {
    double wp = .0;
    double sumRed = .0, sumGreen = .0, sumBlue = .0;
    int h, w;
    double gaussS, gaussT, param;
    /* find the boundary */
    //double sigmaMin = sigmaS < sigmaT ? sigmaS : sigmaT;
    double sigmaMin = 10;
    int hMin = h0-3*sigmaMin > 0 ? h0-3*sigmaMin : 0;
    int hMax = h0+3*sigmaMin < inputPhoto->height ? h0+3*sigmaMin : inputPhoto->height;
    int wMin = w0-3*sigmaMin > 0 ? w0-3*sigmaMin : 0;
    int wMax = w0+3*sigmaMin < inputPhoto->width ? w0+3*sigmaMin : inputPhoto->width;
    /* calc */
    for (h = hMin; h < hMax; h++) {
        for (w = wMin; w < wMax; w++) {
            gaussS = GAUSS_CONST / sigmaS * exp(-distanceSquareSpace(h0, w0, h, w)  / (2.0 * sigmaS * sigmaS));
            gaussT = GAUSS_CONST / sigmaT * exp(-distanceSquareColor(h0, w0, h, w)  / (2.0 * sigmaT * sigmaT));
            param = gaussS * gaussT;
            sumRed += param * inputPhoto->img[h][w].rgbRed;
            sumGreen += param * inputPhoto->img[h][w].rgbGreen;
            sumBlue += param * inputPhoto->img[h][w].rgbBlue;
            wp += param;
        }
    }
    return {sumRed/wp, sumGreen/wp, sumBlue/wp};
}


double BilateralFilter::distanceSquareSpace(int h0, int w0, int h1, int w1) {
    return ((h0-h1)*(h0-h1) + (w0-w1)*(w0-w1));
}


double BilateralFilter::distanceSquareColor(int h0, int w0, int h1, int w1) {
    return ((inputPhoto->img[h0][w0].rgbRed-inputPhoto->img[h1][w1].rgbRed)
             *(inputPhoto->img[h0][w0].rgbRed-inputPhoto->img[h1][w1].rgbRed)
             + (inputPhoto->img[h0][w0].rgbGreen-inputPhoto->img[h1][w1].rgbGreen)
             *(inputPhoto->img[h0][w0].rgbGreen-inputPhoto->img[h1][w1].rgbGreen)
             + (inputPhoto->img[h0][w0].rgbBlue-inputPhoto->img[h1][w1].rgbBlue)
             *(inputPhoto->img[h0][w0].rgbBlue-inputPhoto->img[h1][w1].rgbBlue)
             );
}