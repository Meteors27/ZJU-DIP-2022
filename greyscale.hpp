#ifndef DIP_GREYSCALE_HPP
#define DIP_GREYSCALE_HPP

#include <tuple>
#include "bmp.h"

double rearrange(double x) {
    if (x > 255) {
        return 255.0;
    }
    else if (x < 0) {
        return 0.0;
    }
    return x;
}

std::tuple<double, double, double> RGB2YUV(double r, double g, double b) {
    double y,u,v;
    y = 0.299*r + 0.587*g + 0.114*b;
    u = -0.147*r - 0.289*g + 0.436*b;
    v = 0.615*r - 0.515*g - 0.100*b;
    return {y, u, v};
}

std::tuple<double, double, double> YUV2RGB(double y, double u, double v) {
    double r,g,b;
    r = 1.000*y - 0.000*u + 1.140*v;
    g = 1.000*y - 0.395*u - 0.580*v;
    b = 1.000*y + 2.036*u + 0.000*v;
    return {r, g, b};
}

void greyscale(BMP *bmp) {
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

#endif //DIP_GREYSCALE_HPP
