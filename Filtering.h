#ifndef DIP_FILTERING_H
#define DIP_FILTERING_H

#include "bmp.h"

class Filtering: public BMP{
public:
    PHOTO *meanFiltering(PHOTO* raw);
    PHOTO *meanFiltering(PHOTO* raw, int size);
protected:
    RGBQUAD convolution(PHOTO *img, unsigned int h, unsigned w, int gap);
};

#endif //DIP_FILTERING_H
