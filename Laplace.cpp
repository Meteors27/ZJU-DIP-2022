//
// Created by XMacBook on 2022/11/22.
//

#include "Laplace.h"

PHOTO *Laplace::LaplacianEnhance(PHOTO *raw) {
    auto img = imgnew(raw->width-2, raw->height-2);
    for (int h = 0; h < img->height; h++) {
        for (int w = 0; w < img->width; w++) {
            img->img[h][w] = convolution(raw, h+1, w+1);
        }
    }
    return img;
}

RGBQUAD Laplace::convolution(PHOTO *img, unsigned int h0, unsigned int w0) {
    RGBQUAD res;
    int r=0, g=0, b=0;
    for (int h = h0-1; h <= h0+1; h++) {
        for (int w = w0-1; w <= w0+1; w++) {
            r+=img->img[h][w].rgbRed;
            b+=img->img[h][w].rgbBlue;
            g+=img->img[h][w].rgbGreen;
        }
    }
    res.rgbRed = rearrange(9*img->img[h0][w0].rgbRed - r + img->img[h0][w0].rgbRed);
    res.rgbGreen = rearrange(9*img->img[h0][w0].rgbGreen - r + img->img[h0][w0].rgbGreen);
    res.rgbBlue = rearrange(9*img->img[h0][w0].rgbBlue - r + img->img[h0][w0].rgbBlue);
    res.rgbReserved = 0;
    return res;
}
