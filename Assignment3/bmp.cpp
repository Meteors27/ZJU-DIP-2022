#include "bmp.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#define SUM (InfoHeader.biHeight*InfoHeader.biWidth)
#define imgHeight (InfoHeader.biHeight)
#define imgWidth (InfoHeader.biWidth)
using namespace std;

void BMP::imgread(char *imgpath) {
    cout << "[log] " << "reading img file: \"" << imgpath << "\"" << endl;
    FILE *fp = fopen(imgpath, "r");
    if (!fp) {
        cout << "File not found. " << endl;
        return;
    }
    /* Read bitmap file header */
    fread(&FileHeader, sizeof FileHeader, 1, fp);
    if(FileHeader.bfType[0] != 'B' || FileHeader.bfType[1] != 'M') {
        cerr << "File header incorrect. It must be a bmp file. " << endl;
        return;
    }
    if(FileHeader.bfReserved1 || FileHeader.bfReserved2) {
        cerr << "bfReserved value is not zero." << endl;
        return;
    }
    /* Read bitmap info header */
    fread(&InfoHeader, sizeof InfoHeader, 1, fp);
    if(InfoHeader.biCompression != 0) {
        cerr << "The bmp file is compressed, which cannot be dealt with currently." << endl;
        return;
    }
    cout << "[image] " << "Width: " << InfoHeader.biWidth << " pixels    " << "Height: " << InfoHeader.biHeight << " pixels    " << "BitCount: " << InfoHeader.biBitCount << " bits" << endl;
    height = InfoHeader.biHeight;
    width = InfoHeader.biWidth;
    /* dealing with some exceptions */
    if(FileHeader.bfOffBits != 54) {
        cerr << "Cannot current deal with bmp file with palette. " << endl;
        return;
    }
    if(InfoHeader.biBitCount != 24) {
        cerr << "Cannot current deal with bmp file whose biBitCount is not 24 bits. " << endl;
        return;
    }
    img = new RGBQUAD *[InfoHeader.biHeight];
    if(!img) {
        cerr << "Out of memory. " << endl;
        return;
    }
    int w, h;
    for(h = 0; h < InfoHeader.biHeight; h++) {
        img[h] = new RGBQUAD [InfoHeader.biWidth]; // times 4?
    }
    for(h = 0; h < InfoHeader.biHeight; h++) {
        for(w = 0; w < InfoHeader.biWidth; w++) {
            fread(&img[h][w],InfoHeader.biBitCount/8,1,fp);
        }
        int gap = 0;
        while((w*InfoHeader.biBitCount/8+gap)%4) {
            char c;
            fread(&c, 1, 1, fp);
            gap++;
        }
    }
    cout << "[log] " << "read img file successfully! " <<endl;
    fclose(fp);
}

void BMP::imgwrite(char* imgpath) {
    FILE *fp = fopen(imgpath, "w");
    if (!fp) {
        cout << "Cannot write a new bmp file. " << endl;
        return;
    }
    cout << "[log] " << "writing img file: " << imgpath << endl;
    cout << "[image] " << "Width: " << InfoHeader.biWidth << " pixels    " << "Height: " << InfoHeader.biHeight << " pixels    " << "BitCount: " << InfoHeader.biBitCount << " bits" << endl;
    fwrite(&FileHeader, sizeof FileHeader, 1, fp);
    fwrite(&InfoHeader, sizeof InfoHeader, 1, fp);
    fwrite(Palette, FileHeader.bfOffBits - sizeof(BitMapFileHeader) - sizeof(BitMapInfoHeader), 1, fp);
    int w, h;
    for(h = 0; h < InfoHeader.biHeight; h++) {
        for(w = 0; w < InfoHeader.biWidth; w++) {
            fwrite(&img[h][w],InfoHeader.biBitCount/8,1,fp);
        }
        int gap = 0;
        while((w*InfoHeader.biBitCount/8+gap)% 4) {
            char c = '\0';
            fwrite(&c,1,1,fp);
            gap++;
        }
    }
    fclose(fp);
    cout << "[log] " << "write img file successfully!" << endl;
}

void BMP::greyscale() const {
    int w, h;
    for(h = 0; h < InfoHeader.biHeight; h++) {
        for(w = 0; w < InfoHeader.biWidth; w++) {
            RGBQUAD *pixel = &img[h][w];
            auto [y, u, v] = RGB2YUV(pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
            pixel->rgbBlue = pixel->rgbGreen = pixel->rgbRed = (unsigned char) y;
        }
    }
}

void BMP::binarize(unsigned char threshold) const {
    int w, h;
    for(h = 0; h < InfoHeader.biHeight; h++) {
        for(w = 0; w < InfoHeader.biWidth; w++) {
            RGBQUAD *pixel = &img[h][w];
            if (pixel->rgbRed < threshold) {
                pixel->rgbBlue = pixel->rgbGreen = pixel->rgbRed = 0;
            } else {
                pixel->rgbBlue = pixel->rgbGreen = pixel->rgbRed = 255;
            }
        }
    }
}

std::tuple<double, double, double> BMP::RGB2YUV(double r, double g, double b) {
    double y,u,v;
    y = 0.299*r + 0.587*g + 0.114*b;
    u = -0.147*r - 0.289*g + 0.436*b;
    v = 0.615*r - 0.515*g - 0.100*b;
    return {y, u, v};
}

unsigned char BMP::ostu() const {
    cout << "[log] running ostu algorithm... " << endl;
    auto *gamma = new unsigned char [SUM];
    unsigned char bestThreshold = 0, threshold;
    float max_variance = 0;
    int i = 0;
    for (int h = 0; h < InfoHeader.biHeight; h++) {
        for (int w = 0; w < InfoHeader.biWidth; w++) {
            gamma[i++] = img[h][w].rgbRed;
        }
    }
    unsigned char min_gamma = 255, max_gamma = 0;
    for (i = 0; i < SUM; i++) {
        min_gamma = min(min_gamma, gamma[i]);
        max_gamma = max(max_gamma, gamma[i]);
    }
    for (threshold = min_gamma; threshold < max_gamma; threshold++) {
        // cout << "[ostu] threshold = " << (int)threshold << endl;
        int n1 = 0, n2 = 0;
        long long sum1 = 0, sum2 = 0;
        for (i = 0; i < SUM; i++) {
            if (gamma[i] < threshold) {
                sum1 += gamma[i];
                n1++;
            } else {
                sum2 += gamma[i];
                n2++;
            }
        }
        if (n1 == 0 || n2 == 0) continue;
        float w1 = (float)n1 / SUM, w2 = 1.00 - w1;
        float u1 = (float)sum1 / n1, u2 = (float)sum2 / n2;
        float variance = w1 * w2 * (u1 - u2) * (u1 - u2);
        //  cout << "variance: " << variance << endl;
        if (variance > max_variance) {
            bestThreshold = threshold;
            max_variance = variance;
        }
    }
    cout << "[ostu] best threshold: " << (int)bestThreshold << endl;
    return bestThreshold;
}

void BMP::binarize() {
    binarize(ostu());
}

void BMP::mask(bool choice) const {
    const int H = InfoHeader.biHeight, W = InfoHeader.biWidth;
    bool mask[3][3] = {true,true,true,true,true,true,true,true,true};
    bool gamma[H][W];
    bool erosion[H][W];

    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            gamma[h][w] = (img[h][w].rgbRed != 0);
            if(!choice) gamma[h][w] = !gamma[h][w];
        }
    }
    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            erosion[h][w] = (gamma[max(h-1,0)][max(w-1,0)] || !mask[0][0]) &&
                    (gamma[max(h-1,0)][w] || !mask[0][1]) &&
                    (gamma[max(h-1,0)][min(w+1,W-1)] || !mask[0][2]) &&
                    (gamma[h][max(w-1,0)] || !mask[1][0]) &&
                    (gamma[h][w] || !mask[1][1]) &&
                    (gamma[h][min(w+1,W-1)] || !mask[1][2]) &&
                    (gamma[min(h+1,H-1)][max(w-1,0)] || !mask[2][0]) &&
                    (gamma[min(h+1,H-1)][w] || !mask[2][1]) &&
                    (gamma[min(h+1,H-1)][min(w+1,W-1)] || !mask[2][2]);
        }
    }
    for (int h = 0; h < H; h++) {
        for (int w = 0; w < W; w++) {
            if (choice) {
                img[h][w].rgbRed = img[h][w].rgbGreen = img[h][w].rgbBlue = erosion[h][w] ? 255 : 0;
            }
            else {
                img[h][w].rgbRed = img[h][w].rgbGreen = img[h][w].rgbBlue = erosion[h][w] ? 0 : 255;
            }

        }
    }
}

void BMP::erode() {
    mask(true);
}

void BMP::dilate() {
    mask(false);
}

void BMP::opening() {
    erode();
    dilate();
}

void BMP::closing() {
    dilate();
    erode();
}

std::tuple<double, double, double> BMP::YUV2RGB(double y, double u, double v) {
    double r,g,b;
    r = 1.000*y - 0.000*u + 1.140*v;
    g = 1.000*y - 0.395*u - 0.580*v;
    b = 1.000*y + 2.036*u + 0.000*v;
    return {r, g, b};
}

double BMP::rearrange(double x) {
    if (x > 255) {
        return 255.0;
    }
    else if (x < 0) {
        return 0.0;
    }
    return x;
}

/* Apply visibility enhancement to the img.
 * Directly change the img 2D array in the class.
 */
void BMP::VisEnhance() {
    double maxLumi = 0.00;
    int w, h;
    /* find the maximal luminance in the picture */
    for(h = 0; h < imgHeight; h++) {
        for(w = 0; w < imgWidth; w++) {
            RGBQUAD *pixel = &img[h][w];
            auto [y, u, v] = RGB2YUV(pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
            if (y > maxLumi) maxLumi = y;
        }
    }
    /* logarithmic operation */
    for(h = 0; h < imgHeight; h++) {
        for(w = 0; w < imgWidth; w++) {
            RGBQUAD *pixel = &img[h][w];
            /* transform RGB to YUV */
            auto [y, u, v] = RGB2YUV(pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
            /* rescale luminance Y with logarithmic operation */
            y = 255.00 * log(y/255.00 + 1.00) / log(maxLumi/255.00 + 1.00) ;
            /* transforn YUV to RGB */
            auto [r, g, b] = YUV2RGB(y, u, v);
            /* RGB params need to be rescaled because they may get out of range [0, 255] */
            pixel->rgbRed = rearrange(r);
            pixel->rgbBlue = rearrange(b);
            pixel->rgbGreen = rearrange(g);
        }
    }
}

/* Return a double array[256], which records the histogram of the param.
 * Only support 8bit color */
double *BMP::histogram(Byte **grey) {
    cout << "[log] processing histogram..." << endl;
    long count[256] = {0};
    double res[256];
    /* sum up every greyscale */
    for(int h = 0; h < imgHeight; h++) {
        for(int w = 0; w < imgWidth; w++) {
            count[grey[h][w]]++;
        }
    }
    /* calculate the frequency */
    for (int i = 0; i < 256; i++) {
        res[i] = (double)count[i] / (imgHeight*imgWidth);
    }
    cout << "[log] processing histogram done. " << endl;
    return res;
}

/*  Apply histogram equalization to the input greyscale img.
 *  Directly change the img 2D array in the class.
 */
void BMP::HistogramEq(Byte ** grey) {
    double *raw = histogram(grey);
    double p[256];
    Byte eq[256];
    int i;
    p[0] = raw[0];
    /* calculate the possibility array p[] */
    for (i = 1; i < 256; i++) {
        p[i] = p[i-1] + raw[i];
    }
    /* determine the mapping array eq[] */
    for (i = 0; i < 256; i++) {
        eq[i] = round(p[i] / (1.00/255.00));
    }
    /* write the greyscale data into the img rgb data */
    for(int h = 0; h < imgHeight; h++) {
        for(int w = 0; w < imgWidth; w++) {
            RGBQUAD *pixel = &img[h][w];
            pixel->rgbRed = pixel->rgbBlue = pixel->rgbGreen = eq[grey[h][w]];
        }
    }
}

/* Return a greyscale image (2D array), calculated from the img data */
Byte **BMP::generateGreyscaleImage() {
    cout << "[log] generateGreyscaleImage..." << endl;
    auto greyImg = new Byte* [imgHeight];
    for (int i = 0; i < imgHeight; i++) {
        greyImg[i] = new Byte [imgWidth];
    }
    for(int h = 0; h < imgHeight; h++) {
        for(int w = 0; w < imgWidth; w++) {
            RGBQUAD *pixel = &img[h][w];
            auto [y, u, v] = RGB2YUV(pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
            greyImg[h][w] = (unsigned char) y;
        }
    }
    cout << "[log] generateGreyscaleImage done. " << endl;
    return greyImg;
}