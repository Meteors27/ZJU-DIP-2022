//
// Created by XMacBook on 2022/11/22.
//

#ifndef DIP_LAPLACE_H
#define DIP_LAPLACE_H
#include "bmp.h"

class Laplace : public BMP{
public:
    PHOTO *LaplacianEnhance(PHOTO* raw);
protected:
    RGBQUAD convolution(PHOTO *img, unsigned int h, unsigned int w);
};


#endif //DIP_LAPLACE_H
