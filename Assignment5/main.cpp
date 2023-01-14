#include <iostream>
#include "Laplace.h"
#include "Filtering.h"

int main() {
    Filtering filter;
    Laplace laplace;
    /* MeanFiltering */
    auto img1 = filter.imgread("../images/Matterhorn.bmp");
    auto img2 = filter.meanFiltering(img1, 9);
    filter.imgwrite("../images/MeanFiltering.bmp",img2);
    /* LaplaceEnhancement */
    auto img3 = laplace.imgread("../images/greyscale.bmp");
    auto img4 = laplace.LaplacianEnhance(img3);
    laplace.imgwrite("../images/Laplace.bmp", img4);

    return 0;
}