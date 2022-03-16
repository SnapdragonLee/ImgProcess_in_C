//
// Created by SNAPDRAGON_11800H on 3/14/2022.
//
#include <stdio.h>
#include <stdlib.h>

#include "../settings/settings.h"
#include "bmpIO.h"

bmpFileHeader bmpFHeader;
bmpInfoHeader bmpIHeader;
bmpPixelInfo bmpPx;

void readFile(FILE *bmpFileIn) {
    bmpFHeaderRead(bmpFileIn);

    bmpIHeaderRead(bmpFileIn);
}


void bmpFHeaderRead(FILE *bmpIn) {
    /*printf("bmpFHeader size %llu", sizeof(bmpFHeader));*/

    fread(&bmpFHeader, BmpFHeaderLen, 1, bmpIn);

    if (bmpFHeader.bFType != BM) {
        printf("Not BMP file! Please retry by using other file\n");
        exit(1);
    }

    bmpFHeader.bFSize = (((int) bmpFHeader.bFSize_2) << 16) + bmpFHeader.bFSize_1;
    printf("File size is %u Bytes\n", bmpFHeader.bFSize);

    if (bmpFHeader.bFReserved1 != 0 || bmpFHeader.bFReserved2 != 0) {
        printf("File Format error! Please retry by using other file\n");
        exit(2);
    }

    bmpFHeader.bFOffBits = (((int) bmpFHeader.bFOffBits_2) << 16) + bmpFHeader.bFOffBits_1;
    OffSet = bmpFHeader.bFOffBits;
    printf("The FHeader part is %u Bytes\n", bmpFHeader.bFOffBits);
}

void bmpIHeaderRead(FILE *bmpIn) {
    /*printf("bmpInfoHeader size %llu\n", sizeof(bmpInfoHeader));*/

    fread(&bmpIHeader, BmpIHeaderLen, 1, bmpIn);
    printf("The IHeader Part is %u Bytes\n", bmpIHeader.bISize);

    width = bmpIHeader.bIWidth;
    printf("The image width is %d\n", bmpIHeader.bIWidth);

    height = bmpIHeader.bIHeight;
    printf("The image height is %d\n"
           "The image plane is %hu\n", bmpIHeader.bIHeight, bmpIHeader.bIPlanes);

    if (bmpIHeader.bIBitCount != 24) {
        printf("This file bit count is %hu, may cause errors for some reason\n", bmpIHeader.bIBitCount);
    }

    printf("The compression type is %u\n", bmpIHeader.bICompression);

    printf("The image data is %u Bytes\n", bmpIHeader.bISizeImage);

    printf("The bIXPelsPerMeter is %d\n", bmpIHeader.bIXPelsPerMeter);

    printf("The bIYPelsPerMeter is %d\n", bmpIHeader.bIYPelsPerMeter);

    printf("The bIClrUsed is %u\n", bmpIHeader.bIClrUsed);

    printf("The bIClrImportant is %u\n", bmpIHeader.bIClrImportant);

}

void bmpDataFrame(FILE *bmpData) {
    int i, j = 0;
    int stride;
    unsigned char *pix = NULL;

    stride = (24 * width + 31) / 8;
    stride = stride / 4 * 4;
    pix = malloc(stride);

    for (j = 0; j < height; j++) {
        fread(pix, 1, stride, bmpData);

        for (i = 0; i < width; i++) {
            r[height - 1 - j][i] = pix[i * 3 + 2];
            g[height - 1 - j][i] = pix[i * 3 + 1];
            b[height - 1 - j][i] = pix[i * 3];


            output_r[height - 1 - j][i] = r[height - 1 - j][i];   // 255
            output_g[height - 1 - j][i] = g[height - 1 - j][i];   // 255
            output_b[height - 1 - j][i] = b[height - 1 - j][i];   // 255
        }

    }
}

void writeFile(FILE *bmpFileOut) {
    addHeader(bmpFileOut);

    bmpOutput(bmpFileOut);
}


void addHeader(FILE *bmpFileOut) {
    fwrite(&bmpFHeader, BmpFHeaderLen, 1, bmpFileOut);//输出头文件
    fwrite(&bmpIHeader, BmpIHeaderLen, 1, bmpFileOut);
}

void bmpOutput(FILE *bmpFileOut) {
    long i, j = 0;
    long stride;
    unsigned char *pixout = NULL;

    stride = (24 * width + 31) / 8;
    stride = stride / 4 * 4;
    pixout = malloc(stride);

    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            pixout[i * 3] = output_b[height - 1 - j][i];
            pixout[i * 3 + 1] = output_g[height - 1 - j][i];
            pixout[i * 3 + 2] = output_r[height - 1 - j][i];
        }
        fwrite(pixout, 1, stride, bmpFileOut);
    }
}
