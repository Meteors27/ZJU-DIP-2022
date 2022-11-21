#ifndef DIP_GEOMETRICTRANSFORMATION_H
#define DIP_GEOMETRICTRANSFORMATION_H
#include "bmp.h"

class GeometricTransformation : public BMP {
public:
    void translation();
    void rotation();
    void scale();
    void shear();
    void mirror();
protected:

};


#endif //DIP_GEOMETRICTRANSFORMATION_H
