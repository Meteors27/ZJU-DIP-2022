#include "bmp.h"
#include <iostream>
#include <cstdio>
using namespace std;
PHOTO *BMP::imgread(char *imgpath) {
    cout << "[log] " << "reading img file: \"" << imgpath << "\"" << endl;
    auto *FileHeader = new BitMapFileHeader;
    auto *InfoHeader = new BitMapInfoHeader;
    RGBQUAD **img;
    try {
        FILE *fp = fopen(imgpath, "r");
        if (!fp) throw runtime_error("File not found.");
        /* Read bitmap file header */
        fread(FileHeader, sizeof(BitMapFileHeader), 1, fp);
        if(FileHeader->bfType[0] != 'B' || FileHeader->bfType[1] != 'M') throw runtime_error("File header incorrect. It must be a bmp file.");
        if(FileHeader->bfReserved1 || FileHeader->bfReserved2) throw runtime_error("bfReserved value is not zero.");
        /* Read bitmap info header */
        fread(InfoHeader, sizeof(BitMapInfoHeader), 1, fp);
        if(InfoHeader->biCompression != 0) throw runtime_error("The bmp file is compressed, which cannot be dealt with currently.");
        cout << "[image] " << "Width: " << InfoHeader->biWidth << " pixels    " << "Height: " << InfoHeader->biHeight << " pixels    " << "BitCount: " << InfoHeader->biBitCount << " bits" << endl;
        /* dealing with some exceptions */
        if(FileHeader->bfOffBits != 54) throw runtime_error("Cannot current deal with bmp file with palette.");
        if(InfoHeader->biBitCount != 24) throw runtime_error("Cannot current deal with bmp file whose biBitCount is not 24 bits.");
        /* allocate memory for the image */
        img = new RGBQUAD *[InfoHeader->biHeight];
        int w, h;
        for(h = 0; h < InfoHeader->biHeight; h++) {
            img[h] = new RGBQUAD [InfoHeader->biWidth]; // times 4?
        }
        /* read pixels into the image space */
        for(h = 0; h < InfoHeader->biHeight; h++) {
            for(w = 0; w < InfoHeader->biWidth; w++) {
                fread(&img[h][w],InfoHeader->biBitCount/8,1,fp);
            }
            int gap = 0;
            while((w*InfoHeader->biBitCount/8+gap)%4) {
                char c;
                fread(&c, 1, 1, fp);
                gap++;
            }
        }
        fclose(fp);
        cout << "[log] " << "read img file successfully! " <<endl;
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        return nullptr;
    }
    /* create photo structure */
    auto *photo = new PHOTO;
    photo->fileHeader = FileHeader;
    photo->infoHeader = InfoHeader;
    photo->img = img;
    photo->height = InfoHeader->biHeight;
    photo->width = InfoHeader->biWidth;
    return photo;
}

void BMP::imgwrite(char* imgpath, PHOTO *photo) {
    FILE *fp = fopen(imgpath, "w");
    if (!fp) {
        cout << "Cannot write a new bmp file. " << endl;
        return;
    }
    cout << "[log] " << "writing img file: " << imgpath << endl;
    cout << "[image] " << "Width: " << photo->infoHeader->biWidth << " pixels    " << "Height: " << photo->infoHeader->biHeight << " pixels    " << "BitCount: " << photo->infoHeader->biBitCount << " bits" << endl;
    fwrite(photo->fileHeader, sizeof(BitMapFileHeader), 1, fp);
    fwrite(photo->infoHeader, sizeof(BitMapInfoHeader), 1, fp);
    int w, h;
    for(h = 0; h < photo->infoHeader->biHeight; h++) {
        for(w = 0; w < photo->infoHeader->biWidth; w++) {
            fwrite(&photo->img[h][w],photo->infoHeader->biBitCount/8,1,fp);
        }
        int gap = 0;
        while((w*photo->infoHeader->biBitCount/8+gap)% 4) {
            char c = '\0';
            fwrite(&c,1,1,fp);
            gap++;
        }
    }
    fclose(fp);
    cout << "[log] " << "write img file successfully!" << endl;
}

// TODO: 为什么有时候会有 2 bytes 的差异？
PHOTO *BMP::imgnew(unsigned int width, unsigned int height) {
    /* File Header */
    auto photo = new PHOTO;
    photo->width = width;
    photo->height = height;
    photo->fileHeader = new BitMapFileHeader;
    photo->fileHeader->bfType[0] = 'B';
    photo->fileHeader->bfType[1] = 'M';
    photo->fileHeader->bfSize = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + height * ((3+3*photo->width)/4*4); // TODO: probably not correct
    photo->fileHeader->bfReserved1 = 0;
    photo->fileHeader->bfReserved2 = 0;
    photo->fileHeader->bfOffBits = sizeof(BitMapFileHeader)+sizeof(BitMapInfoHeader);
    /* Info Header */
    photo->infoHeader = new BitMapInfoHeader;
    photo->infoHeader->biSize = sizeof (BitMapInfoHeader);
    photo->infoHeader->biWidth = width;
    photo->infoHeader->biHeight = height;
    photo->infoHeader->biPlanes = 1;
    photo->infoHeader->biBitCount = 24;
    photo->infoHeader->biCompression = 0;
    photo->infoHeader->biSizeImage = photo->fileHeader->bfSize - photo->fileHeader->bfOffBits;
    photo->infoHeader->biXPelsPerMeter = 0;
    photo->infoHeader->biYPelsPerMeter = 0;
    photo->infoHeader->biClrUsed = 0; // TODO
    photo->infoHeader->biClrImportant = 0; // TODO
    /* img */
    photo->img = new RGBQUAD *[height];
    int w, h;
    for(h = 0; h < height; h++) {
        photo->img[h] = new RGBQUAD [width];
    }
    for (h = 0; h < height; h++) {
        for (w = 0; w < width; w++) {
            photo->img[h][w].rgbGreen = photo->img[h][w].rgbBlue = photo->img[h][w].rgbRed = photo->img[h][w].rgbReserved = 0;
        }
    }
    return photo;
}

std::tuple<double, double, double> BMP::RGB2YUV(double r, double g, double b) {
    double y,u,v;
    y = 0.299*r + 0.587*g + 0.114*b;
    u = -0.147*r - 0.289*g + 0.436*b;
    v = 0.615*r - 0.515*g - 0.100*b;
    return {y, u, v};
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