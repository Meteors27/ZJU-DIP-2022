#ifndef DIP_BMP_H
#define DIP_BMP_H
#include <tuple>
typedef unsigned char Byte;
#pragma pack(1)
typedef struct TagBitMapFileHeader
{
    unsigned char  bfType[2];     //文件格式（0-1字节）
    unsigned int   bfSize;        // 文件大小 以字节为单位(2-5字节)
    unsigned short bfReserved1;   // 保留字 (6-7字节)
    unsigned short bfReserved2;   // 保留字 (8-9字节)
    unsigned int   bfOffBits;     // 从文件头到像素数据的偏移  (10-13字节)
}BitMapFileHeader;
#pragma pack()
#pragma pack(1)
typedef struct TagBitMapInfoHeader
{
    unsigned int    biSize;          // 此结构体的大小 (14-17字节)
    unsigned int    biWidth;         // 图像的宽  (18-21字节)
    unsigned int    biHeight;        // 图像的高  (22-25字节)
    unsigned short  biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short  biBitCount;      // 一像素所占的位数，(28-29字节)当biBitCount=24时，该BMP图像就是24Bit真彩图，没有调色板项。
    unsigned int    biCompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
    unsigned int    biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
    unsigned int    biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
    unsigned int    biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    unsigned int    biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数。 (46-49字节)
    unsigned int    biClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
}BitMapInfoHeader;
#pragma pack()
#pragma pack(1)
typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;   //该颜色的蓝色分量  (值范围为0-255)
    unsigned char rgbGreen;  //该颜色的绿色分量  (值范围为0-255)
    unsigned char rgbRed;    //该颜色的红色分量  (值范围为0-255)
    unsigned char rgbReserved;// 保留字
}RGBQUAD;
#pragma pack()
#pragma pack(1)
typedef struct YUV
{
    unsigned char yuvY;   //该颜色的蓝色分量  (值范围为0-255)
    unsigned char yuvU;  //该颜色的绿色分量  (值范围为0-255)
    unsigned char yuvV;    //该颜色的红色分量  (值范围为0-255)
}PixelYUV;
#pragma pack()

class BMP {
public:
    BMP(){Palette= nullptr;}
    void imgread(char* imgpath);                            // 读取bmp文件
    void imgwrite(char* imgpath);                           // 写入bmp文件
    RGBQUAD **img;                                          // 存储图像数据
    // PixelYUV **imgYUV;
    unsigned int height;                                    // 图像的高
    unsigned int width;                                     // 图像的宽
    void greyscale() const;
    void binarize();
    void erode();
    void dilate();
    void opening();
    void closing();
    void VisEnhance();
    void HistogramEq(Byte **);
    Byte **generateGreyscaleImage();
private:
    BitMapFileHeader FileHeader;
    BitMapInfoHeader InfoHeader;
    void *Palette;
    unsigned char ostu() const;
    void binarize(unsigned char threshold) const;
    static std::tuple<double, double, double> RGB2YUV(double r, double g, double b);
    static std::tuple<double, double, double> YUV2RGB(double y, double u, double v);
    double rearrange(double x);
    double *histogram(Byte **);
    void mask(bool) const; // erosion和dilation的共有代码片段, true代表erosion, false代表dilation
};

#endif //DIP_BMP_H
