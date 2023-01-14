#include <iostream>
#include "bmp.h"
using namespace std;

int main() {
    BMP bmp;
    char mountain[] = "../images/cliff.bmp";
    char enhance[] = "../images/VisEnhance_cliff.bmp";
    char eq[] = "../images/equalization_cliff.bmp";
    bmp.imgread(mountain);
    bmp.VisEnhance();
    bmp.imgwrite(enhance);
    bmp.imgread(mountain);
    auto greyImg = bmp.generateGreyscaleImage();
    bmp.HistogramEq(greyImg);
    bmp.imgwrite(eq);
    bmp.imgread(mountain);
    bmp.greyscale();
    bmp.imgwrite("../images/greyscale_cliff.bmp");

    return 0;
}

//int main() {
//    BMP bmp;
//    char mountain[] = "../images/Light.bmp";
//    char enhance[] = "../images/VisEnhance.bmp";
//    bmp.imgread(mountain);
//    int w, h;
//    for(h = 0; h < bmp.InfoHeader.biHeight; h++) {
//        for(w = 0; w < bmp.InfoHeader.biWidth; w++) {
//            RGBQUAD *pixel = &bmp.img[h][w];
//            auto [y, u, v] = bmp.RGB2YUV(pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
//            auto [r,g,b] = bmp.YUV2RGB(y+100,u,v);
//            pixel->rgbBlue = bmp.rearrange(b);
//            pixel->rgbGreen = bmp.rearrange(g);
//            pixel->rgbRed = bmp.rearrange(r);
//        }
//    }
//    bmp.imgwrite("../images/Light.bmp");
//    return 0;
//}
