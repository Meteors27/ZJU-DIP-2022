#include "BilateralFilter.h"

int main() {
    BilateralFilter filter(12, 1);
    /* Bilateral Filtering */
    auto input = filter.imgread("../images/castle.bmp");
    auto output = filter.bf(input);
    filter.imgwrite("../images/BilateralFilter.bmp",output);
    return 0;
}