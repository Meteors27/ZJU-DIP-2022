#include <iostream>
#include "bmp.h"

// using namespace std;

int main() {
    BMP bmp;
    std::cout << "hello" << std::endl;
    char mountain[] = "../images/Matterhorn.bmp";
    auto img = bmp.imgread(mountain);
    bmp.imgwrite("../images/test.bmp", img);

    return 0;
}