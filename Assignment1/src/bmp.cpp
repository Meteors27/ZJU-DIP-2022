#include "bmp.h"
#include <iostream>
#include <stdio.h>
using namespace std;

void BMP::imgread(char *imgpath) {
    cout << "[log] " << "reading img file..." << endl;
    FILE *fp = NULL;
    fp = fopen(imgpath, "r");
    if (!fp) {
        cout << "File not found. " << endl;
        return;
    }
    /* Read bitmap file header */
    fread(FileHeader.bfType, 1, 2, fp);
    if(FileHeader.bfType[0] != 'B' || FileHeader.bfType[1] != 'M') {
        cerr << "File header incorrect. It must be a bmp file. " << endl;
        return;
    }
    fread(&FileHeader.bfSize, 1, 4, fp);
    fread(&FileHeader.bfReserved1, 1, 2, fp);
    fread(&FileHeader.bfReserved2, 1, 2, fp);
    if(FileHeader.bfReserved1 || FileHeader.bfReserved2) {
        cerr << "bfReserved value is not zero." << endl;
        return;
    }
    fread(&FileHeader.bfOffBits, 1, 4, fp);
    /* Read bitmap info header */
    fread(&InfoHeader.biSize, 1, 4, fp);
    fread(&InfoHeader.biWidth, 1, 4, fp);
    fread(&InfoHeader.biHeight, 1, 4, fp);
    fread(&InfoHeader.biPlanes, 1, 2, fp);
    fread(&InfoHeader.biBitCount, 1, 2, fp);
    fread(&InfoHeader.biCompression, 1, 4, fp);
    if(InfoHeader.biCompression != 0) {
        cerr << "The bmp file is compressed, which cannot be dealt with currently." << endl;
        return;
    }
    fread(&InfoHeader.biSizeImage, 1, 4, fp);
    fread(&InfoHeader.biXPelsPerMeter, 1, 4, fp);
    fread(&InfoHeader.biYPelsPerMeter, 1, 4, fp);
    fread(&InfoHeader.biClrUsed, 1, 4, fp);
    fread(&InfoHeader.biClrImportant, 1, 4, fp);
    cout << "[image info] " << "Width: " << InfoHeader.biWidth << " pixels    " << "Height: " << InfoHeader.biHeight << " pixels    " << "BitCount: " << InfoHeader.biBitCount << " bits" << endl;
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
    img = new RGBQUAD *[InfoHeader.biWidth];
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
            fread(&img[h][w].rgbBlue,1,1,fp);
            fread(&img[h][w].rgbGreen,1,1,fp);
            fread(&img[h][w].rgbRed,1,1,fp);
        }
        while(w%4) {
            fgetc(fp);
            w++;
        }
    }
    if(ftell(fp) == FileHeader.bfSize) {
        cout << "[log] " << "read img file successfully! " <<endl;
    }
    else {
        cerr << "ended when not entirely read. " << endl;
    }
    fclose(fp);
}

void BMP::imgwrite(char* imgpath) {
    FILE *fp = NULL;
    fp = fopen(imgpath, "w");
    if (!fp) {
        cout << "Cannot write a new bmp file. " << endl;
        return;
    }
    cout << "[log] " << "writing img file..." << endl;
    //    fwrite(&FileHeader, sizeof FileHeader, 1, fp);
    //    fwrite(&InfoHeader, sizeof InfoHeader, 1, fp);
    fwrite(FileHeader.bfType, 1, 2, fp);
    fwrite(&FileHeader.bfSize, 1, 4, fp);
    fwrite(&FileHeader.bfReserved1, 1, 2, fp);
    fwrite(&FileHeader.bfReserved2, 1, 2, fp);
    fwrite(&FileHeader.bfOffBits, 1, 4, fp);

    fwrite(&InfoHeader.biSize, 1, 4, fp);
    fwrite(&InfoHeader.biWidth, 1, 4, fp);
    fwrite(&InfoHeader.biHeight, 1, 4, fp);
    fwrite(&InfoHeader.biPlanes, 1, 2, fp);
    fwrite(&InfoHeader.biBitCount, 1, 2, fp);
    fwrite(&InfoHeader.biCompression, 1, 4, fp);
    fwrite(&InfoHeader.biSizeImage, 1, 4, fp);
    fwrite(&InfoHeader.biXPelsPerMeter, 1, 4, fp);
    fwrite(&InfoHeader.biYPelsPerMeter, 1, 4, fp);
    fwrite(&InfoHeader.biClrUsed, 1, 4, fp);
    fwrite(&InfoHeader.biClrImportant, 1, 4, fp);

    int w, h;
    for(h = 0; h < InfoHeader.biHeight; h++) {
        for(w = 0; w < InfoHeader.biWidth; w++) {
            fwrite(&img[h][w].rgbBlue,1,1,fp);
            fwrite(&img[h][w].rgbGreen,1,1,fp);
            fwrite(&img[h][w].rgbRed,1,1,fp);
        }
        while(w % 4) {
            fputc('0',fp);
            w++;
        }
    }
    fclose(fp);
    cout << "[log] " << "write img file successfully!" << endl;
}

void BMP::renderbypixel(RGBQUAD (*callback)(RGBQUAD)) {
    int h,w;
    for(h = 0; h < InfoHeader.biHeight; h++) {
        for(w = 0; w < InfoHeader.biWidth; w++) {
            img[h][w] = callback(img[h][w]);
        }
    }
}