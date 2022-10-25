#include <iostream>
#include "bmp.h"
using namespace std;

int main() {
    BMP bmp;
    char input[] = "../images/Matterhorn.bmp";
    char output[] = "../images/test.bmp";
    bmp.imgread(input);
    bmp.VisEnhance();
    bmp.imgwrite(output);
    return 0;
}
