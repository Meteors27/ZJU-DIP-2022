#include <iostream>
#include "bmp.h"
using namespace std;

int main() {
    BMP bmp;
    char input[] = "../images/Matterhorn.bmp";
    char binary[] = "../images/binary.bmp";
    char OpeningClosing[] = "../images/OpeningClosing.bmp";
    bmp.imgread(input);
    bmp.greyscale();
    bmp.binarize();
    bmp.imgwrite(binary);
    bmp.opening(1);
    bmp.closing(1);
    bmp.imgwrite(OpeningClosing);
    return 0;
}
