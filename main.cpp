#include <iostream>
#include "bmp.h"
using namespace std;

int main() {
    BMP bmp;
    char input[] = "../images/Matterhorn.bmp";
    char output[] = "../images/output.bmp";
    bmp.imgread(input);
    bmp.greyscale();
    bmp.binarize();
    bmp.imgwrite(output);
    return 0;
}
