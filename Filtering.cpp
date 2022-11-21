//
// Created by XMacBook on 2022/11/21.
//

#include "Filtering.h"
#include <iostream>
#include <cmath>

PHOTO *Filtering::meanFiltering(PHOTO *raw) {
    return meanFiltering(raw, 3);
}

PHOTO *Filtering::meanFiltering(PHOTO *raw, int size) {
    auto img = imgnew(raw->width-size+1, raw->height-size+1);
    int gap = size/2;
    for (int h = 0; h < img->height; h++) {
        for (int w = 0; w < img->width; w++) {
            img->img[h][w] = convolution(raw, h+gap, w+gap, size);
        }
    }
    return img;
}

RGBQUAD Filtering::convolution(PHOTO *img, unsigned int h0, unsigned int w0, int size) {
    /* size > 0 && size%2 == 1 */
    RGBQUAD res;
    int gap = size/2;
    int r=0, g=0, b=0;
    for (int h = h0-gap; h <= h0+gap; h++) {
        for (int w = w0-gap; w <= w0+gap; w++) {
            r+=img->img[h][w].rgbRed;
            b+=img->img[h][w].rgbBlue;
            g+=img->img[h][w].rgbGreen;
        }
    }
    // TODO: 类型转换
    res.rgbRed = round(r/(1.0*size*size));
    res.rgbGreen = round(g/(1.0*size*size));
    res.rgbBlue = round(b/(1.0*size*size));
    res.rgbReserved = 0;
    return res;
}