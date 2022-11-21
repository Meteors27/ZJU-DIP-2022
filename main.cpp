#include <iostream>
#include "Laplace.h"

int main() {
    Laplace bmp;
    std:: cout << "hello" << std::endl;
    char mountain[] = "../images/greyscale.bmp";
    auto img = bmp.imgread(mountain);
    auto img2 = bmp.LaplacianEnhance(img);
    bmp.imgwrite("../images/test.bmp", img2);
    return 0;
}