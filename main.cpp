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
    bmp.opening();
    bmp.closing();
    //for (int i = 0; i < 1; i ++) bmp.dilate();
//    for (int i = 0; i < 10; i ++) bmp.erode();
//    for (int i = 0; i < 11; i ++) bmp.dilate();
//    for (int i = 0; i < 10; i ++) bmp.erode();
//    for (int i = 0; i < 10; i ++) bmp.dilate();
    bmp.imgwrite(OpeningClosing);
    return 0;
}
