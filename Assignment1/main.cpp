#include <iostream>
#include "src/bmp.h"
#define DELTA -100 //亮度的变化值，作用在color.bmp中
using namespace std;

tuple<double, double, double> RGB2YUV(double, double, double); // 将RGB色彩空间映射到YUV色彩空间
tuple<double, double, double> YUV2RGB(double, double, double); // 将YUV色彩空间映射到RGB色彩空间
double rearrange(double);
RGBQUAD greyscale(RGBQUAD pixel); // 将输入的单个像素进行灰度转换
RGBQUAD luminance(RGBQUAD pixel); // 将输入的单个像素进行亮度转换

int main() {
    BMP test1;
    char input[] = "../devices.bmp";
    char output1[] = "../greyscale.bmp";
    char output2[] = "../color.bmp";
    test1.imgread(input); // 从指定路径读取bmp文件
    test1.renderbypixel(greyscale); // 使用自定义的函数对每一个像素进行渲染，这里进行灰度转换
    test1.imgwrite(output1); // 输出bmp文件到指定路径
    test1.imgread(input);
    test1.renderbypixel(luminance);
    test1.imgwrite(output2);
    return 0;
}

RGBQUAD greyscale(RGBQUAD pixel) {
    auto [y,u,v] = RGB2YUV(pixel.rgbRed,pixel.rgbGreen,pixel.rgbBlue);
    auto [r,g,b] = YUV2RGB(y,0,0);
    RGBQUAD result;
    result.rgbRed = (unsigned char)rearrange(r);
    result.rgbGreen = (unsigned char)rearrange(g);
    result.rgbBlue = (unsigned char)rearrange(b);
    return result;
}

RGBQUAD luminance(RGBQUAD pixel) {
    auto [y,u,v] = RGB2YUV(pixel.rgbRed,pixel.rgbGreen,pixel.rgbBlue);
    auto [r,g,b] = YUV2RGB(y+DELTA,u,v);
    RGBQUAD result;
    result.rgbRed = (unsigned char)rearrange(r);
    result.rgbGreen = (unsigned char)rearrange(g);
    result.rgbBlue = (unsigned char)rearrange(b);
    return result;
}

double rearrange(double x) {
    if (x > 255) {
        return 255.0;
    }
    else if (x < 0) {
        return 0.0;
    }
    return x;
}

tuple<double, double, double> RGB2YUV(double r, double g, double b) {
    double y,u,v;
    y = 0.299*r + 0.587*g + 0.114*b;
    u = -0.147*r - 0.289*g + 0.436*b;
    v = 0.615*r - 0.515*g - 0.100*b;
    return {y, u, v};
}

tuple<double, double, double> YUV2RGB(double y, double u, double v) {
    double r,g,b;
    r = 1.000*y - 0.000*u + 1.140*v;
    g = 1.000*y - 0.395*u - 0.580*v;
    b = 1.000*y + 2.036*u + 0.000*v;
    return {r, g, b};
}
