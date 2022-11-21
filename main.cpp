#include <iostream>
#include "Filtering.h"

// using namespace std;

int main() {
    Filtering bmp;
    char mountain[] = "../images/Matterhorn.bmp";
    auto img = bmp.imgread(mountain);
    auto img2 = bmp.meanFiltering(img, 11);
    bmp.imgwrite("../images/test.bmp", img2);

    return 0;
}