#ifndef DIP_BINARIZE_HPP
#define DIP_BINARIZE_HPP
#include "bmp.h"
#include "greyscale.hpp"
void binarize(BMP *bmp, unsigned char threshold) {
    for (int h = 0; h < bmp->height; h++) {
        for (int w = 0; w < bmp->width; w++) {
            RGBQUAD *pixel = &bmp->img[h][w];
            auto [y,u,v] = RGB2YUV(pixel->rgbRed,pixel->rgbGreen,pixel->rgbBlue);
            auto [r,g,b] = YUV2RGB(y,0,0);
            pixel->rgbRed = (unsigned char)rearrange(r);
            pixel->rgbGreen = (unsigned char)rearrange(g);
            pixel->rgbBlue = (unsigned char)rearrange(b);
        }
    }
}

#endif //DIP_BINARIZE_HPP
