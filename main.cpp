#include <iostream>
#include "bmp.h"
#include "greyscale.hpp"
using namespace std;

void ostu() {

}

int main() {
    BMP bmp;
    char input[] = "../images/Matterhorn.bmp";
    char output[] = "../images/output.bmp";
    bmp.imgread(input);
    greyscale(&bmp);
    bmp.imgwrite(output);
    return 0;
}
