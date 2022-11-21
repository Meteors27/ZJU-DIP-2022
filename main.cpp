#include <iostream>
#include "Filtering.h"

int main() {
    Filtering bmp;
    char mountain[] = "../images/Matterhorn.bmp";
    auto img = bmp.imgread(mountain);
    auto img2 = bmp.meanFiltering(img, 9);
    bmp.imgwrite("../images/MeanFiltering.bmp", img2);
    return 0;
}