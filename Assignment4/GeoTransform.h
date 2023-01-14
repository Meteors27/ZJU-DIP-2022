#ifndef GEOMETRICTRANSFORMATION_GEOTRANSFORM_H
#define GEOMETRICTRANSFORMATION_GEOTRANSFORM_H
#include "bmp.h"

class GeoTransform : public BMP{
public:
    // TODO: what if raw == nullptr?
    static PHOTO *translation(PHOTO *raw, int x, int y);
    PHOTO *rotation(PHOTO *raw, double angle);
    PHOTO *scale(PHOTO *raw, double coefficient);
    PHOTO *scale(PHOTO *raw, double coefficient_height, double coefficient_width);
    PHOTO *shear(PHOTO *raw, char axis, double delta);
    PHOTO *mirror(PHOTO *raw, char axis);
protected:
    RGBQUAD bilinearInterpolition(PHOTO *raw, double h, double w);
};

#endif //GEOMETRICTRANSFORMATION_GEOTRANSFORM_H
