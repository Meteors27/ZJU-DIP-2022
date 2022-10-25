#include <iostream>
#include "bmp.h"
using namespace std;

int main() {
    BMP bmp;
    char mountain[] = "../images/Matterhorn.bmp";
    char enhance[] = "../images/VisEnhance.bmp";
    char eq[] = "../images/equalization.bmp";
    bmp.imgread(mountain);
    bmp.VisEnhance();
    bmp.imgwrite(enhance);
    bmp.imgread(mountain);
    auto greyImg = bmp.generateGreyscaleImage();
    bmp.HistogramEq(greyImg);
    bmp.imgwrite(eq);
    return 0;
}
