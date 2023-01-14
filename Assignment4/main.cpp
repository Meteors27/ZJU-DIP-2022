#include <iostream>
#include "GeoTransform.h"
using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
    char input[] = "../images/devices.bmp";
    char out[] = "../images/mirror2.bmp";
    GeoTransform bmp;
    auto raw = bmp.imgread(input);
    auto processed = bmp.mirror(raw, 'y');
    bmp.imgwrite(out, processed);

    /*
    cout << photo->infoHeader->biHeight * ((3+3*photo->infoHeader->biWidth)/4*4) << endl;
    cout << sizeof(BitMapFileHeader)+sizeof(BitMapInfoHeader) + photo->infoHeader->biHeight * ((3+3*photo->infoHeader->biWidth)/4*4) << endl;
    cout << photo->infoHeader->biSizeImage << endl;
    cout << photo->fileHeader->bfSize << endl;
    */
    return 0;
}
