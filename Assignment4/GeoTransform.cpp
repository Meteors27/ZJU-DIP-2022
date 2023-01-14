#include "GeoTransform.h"
#include <cmath>
#include <iostream>
#define pi 3.14159
PHOTO *GeoTransform::translation(PHOTO *raw, int x, int y) {
    auto photo = imgnew(raw->width + abs(x), raw->height + abs(y));
    for (int h = 0; h < raw->height; h++) {
        for (int w = 0; w < raw->width; w++) {
            photo->img[h+x][w+y] = raw->img[h][w]; // TODO: a little modification
        }
    }
    return photo;
}

PHOTO *GeoTransform::scale(PHOTO *raw, double coefficient) {
    if (coefficient <= 0) {
        std::cout << "GeoTransform scale: coefficient must be above zero!" << std::endl;
        return nullptr;
    }
    else if (coefficient <= 1) {
        unsigned int scaled_width = floor(coefficient * raw->width);
        unsigned int scaled_height = floor(coefficient * raw->height);
        auto photo = imgnew(scaled_width, scaled_height);
        for (int h = 0; h < scaled_height; h++) {
            for (int w = 0; w < scaled_width; w++) {
                photo->img[h][w] = raw->img[(unsigned int)floor(h/coefficient)][(unsigned int)floor(w/coefficient)]; // TODO
            }
        }
        return photo;
    }
    else {
        unsigned int scaled_width = floor(coefficient * raw->width);
        unsigned int scaled_height = floor(coefficient * raw->height);
        auto photo = imgnew(scaled_width, scaled_height);
        for (int h = 0; h < scaled_height; h++) {
            for (int w = 0; w < scaled_width; w++) {
                photo->img[h][w] = bilinearInterpolition(raw, h/coefficient, w/coefficient);
            }
        }
        std::cout << "bilinearInterpolition success! " << std::endl;
        return photo;
    }
}

PHOTO *GeoTransform::scale(PHOTO *raw, double coefficient_height, double coefficient_width) {
    if (coefficient_height <= 0 || coefficient_width <= 0) {
        std::cout << "GeoTransform scale: coefficient must be above zero!" << std::endl;
        return nullptr;
    }
    //photo->img[h][w] = raw->img[(unsigned int)floor(h/coefficient)][(unsigned int)floor(w/coefficient)]; // TODO
    unsigned int scaled_width = floor(coefficient_width * raw->width);
    unsigned int scaled_height = floor(coefficient_height * raw->height);
    auto photo = imgnew(scaled_width, scaled_height);
    double h0, w0;
    for (int h = 0; h < scaled_height; h++) {
        for (int w = 0; w < scaled_width; w++) {
            if (coefficient_height <= 1) {
                h0 = floor(h/coefficient_height);
            } else {
                h0 = h/coefficient_height;
            }
            if (coefficient_width <= 1) {
                w0 = floor(w/coefficient_width);
            } else {
                w0 = w/coefficient_width;
            }
            photo->img[h][w] = bilinearInterpolition(raw, h0, w0);
        }
    }
    std::cout << "bilinearInterpolition success! " << std::endl;
    return photo;
}

PHOTO *GeoTransform::shear(PHOTO *raw, char axis, double delta) {
    if (axis != 'x' && axis != 'y') {
        std::cout << "GeoTransform shear: invalid axis name." << std::endl;
        return nullptr;
    }
    if (axis == 'x') {
        auto photo = imgnew(raw->width, floor(raw->height+delta*raw->width));
        for (int h = 0; h < raw->height; h++) {
            for (int w = 0; w < raw->width; w++) {
                photo->img[(int)floor(h+delta*w)][w] = raw->img[h][w];
            }
        }
        return photo;
    }
    else {
        auto photo = imgnew(floor(raw->width+delta*raw->height), raw->height);
        for (int h = 0; h < raw->height; h++) {
            for (int w = 0; w < raw->width; w++) {
                photo->img[h][(int)floor(w+delta*h)] = raw->img[h][w];
            }
        }
        return photo;
    }
}

PHOTO *GeoTransform::mirror(PHOTO *raw, char axis) {
    if (axis != 'x' && axis != 'y') {
        std::cout << "GeoTransform shear: invalid axis name." << std::endl;
        return nullptr;
    }
    if (axis == 'x') {
        auto photo = imgnew(2 * raw->width, raw->height);
        for (int h = 0; h < raw->height; h++) {
            for (int w = 0; w < raw->width; w++) {
                photo->img[h][w] = raw->img[h][raw->width-1-w];
            }
        }
        return photo;
    }
    else {
        auto photo = imgnew(raw->width, 2 * raw->height);
        for (int h = 0; h < raw->height; h++) {
            for (int w = 0; w < raw->width; w++) {
                photo->img[h][w] = raw->img[raw->height - 1 - h][w];
            }
        }
        return photo;
    }
}

PHOTO *GeoTransform::rotation(PHOTO *raw, double angle) {
    double width = floor(abs(raw->width * cos(angle/180.0*pi)) + abs(raw->height * sin(angle/180.0*pi)));
    double height = floor(abs(raw->height * cos(angle/180.0*pi)) + abs(raw->width * sin(angle/180.0*pi)));
    auto photo = imgnew(round(width), round(height));
    int h, w;
    double h0, w0;
    for (h = 0; h < height; h++) {
        for (w = 0; w < width; w++) {
            h0 = (h - height / 2.0) * cos(angle/180.0*pi) + (w - width / 2.0) * sin(angle/180.0*pi) + raw->height / 2.0;
            w0 = (h - height / 2.0) * sin(-angle/180.0*pi) + (w - width / 2.0) * cos(angle/180.0*pi) + raw->width / 2.0;
            if (0 <= h0 && h0 < raw->height && 0 <= w0 && w0 < raw->width) { // inside the raw image
                photo->img[h][w] = bilinearInterpolition(raw, h0, w0);
            }
        }
    }
    return photo;
}

RGBQUAD GeoTransform::bilinearInterpolition(PHOTO *raw, double h, double w) {
    /*  - -
     * |1 2|
     * |3 4|
     *  - -
     */
    // std::cout << "start in point: " << h << ", " << w << std::endl;
    h = (h > raw->height - 1) ? raw->height - 1 : h;
    w = (w > raw->width - 1) ? raw->width - 1 : w;
    RGBQUAD point1 = raw->img[(int)floor(h)][(int)floor(w)];
    RGBQUAD point2 = raw->img[(int)floor(h)][(int)ceil(w)];
    RGBQUAD point3 = raw->img[(int)ceil(h)][(int)floor(w)];
    RGBQUAD point4 = raw->img[(int)ceil(h)][(int)ceil(w)];
    RGBQUAD ans;
    double h0 = h - floor(h);
    double w0 = w - floor(w);

    ans.rgbRed = (point3.rgbRed-point1.rgbRed)*h0 + (point2.rgbRed-point1.rgbRed)*w0 + (point1.rgbRed+point4.rgbRed-point2.rgbRed-point3.rgbRed)*h0*w0 + point1.rgbRed;
    ans.rgbBlue = (point3.rgbBlue-point1.rgbBlue)*h0 + (point2.rgbBlue-point1.rgbBlue)*w0 + (point1.rgbBlue+point4.rgbBlue-point2.rgbBlue-point3.rgbBlue)*h0*w0 + point1.rgbBlue;
    ans.rgbGreen = (point3.rgbGreen-point1.rgbGreen)*h0 + (point2.rgbGreen-point1.rgbGreen)*w0 + (point1.rgbGreen+point4.rgbGreen-point2.rgbGreen-point3.rgbGreen)*h0*w0 + point1.rgbGreen;
    ans.rgbReserved = 0;
    // std::cout << "success in point: " << h << ", " << w << std::endl;
    return ans;
}