//
// Created by SNAPDRAGON_11800H on 3/14/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../settings/settings.h"
#include "bmpIO.h"

bmpFileHeader bmpFHeader;
bmpInfoHeader bmpIHeader;
bmpPixelInfo bmpPx;

unsigned char *otherInfo = NULL;

void skipPixelInfo();

void readFile(FILE *bmpFileIn) {
    bmpFHeaderRead(bmpFileIn);
    bmpIHeaderRead(bmpFileIn);

    skipPixelInfo(bmpFileIn);
}


void bmpFHeaderRead(FILE *bmpIn) {
    /*printf("bmpFHeader size %llu", sizeof(bmpFHeader));*/

    fread(&bmpFHeader, BmpFHeaderLen, 1, bmpIn);

    if (bmpFHeader.bFType != BM) {
        printf("Not BMP file! Please retry by using other file\n");
        exit(1);
    }

    bmpFHeader.bFSize = (bmpFHeader.bFSize_2 << 16) + bmpFHeader.bFSize_1;
    printf("File size is %u Bytes\n", bmpFHeader.bFSize);

    if (bmpFHeader.bFReserved1 != 0 || bmpFHeader.bFReserved2 != 0) {
        printf("File Format error! Please retry by using other file\n");
        exit(2);
    }

    bmpFHeader.bFOffBits = (bmpFHeader.bFOffBits_2 << 16) + bmpFHeader.bFOffBits_1;
    printf("The FHeader part is %u Bytes\n", bmpFHeader.bFOffBits);
}

void bmpIHeaderRead(FILE *bmpIn) {
    /*printf("bmpInfoHeader size %llu\n", sizeof(bmpInfoHeader));*/

    fread(&bmpIHeader, BmpIHeaderLen, 1, bmpIn);

    printf("The IHeader Part is %u Bytes\n", bmpIHeader.bISize);

    printf("The image width is %d\n", bmpIHeader.bIWidth);

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

void skipPixelInfo(FILE *bmpIn) {
    if (bmpFHeader.bFOffBits - BmpHeaderLen != 0) {
        otherInfo = (unsigned char *) malloc(bmpFHeader.bFOffBits - BmpHeaderLen);
        fread(otherInfo, bmpFHeader.bFOffBits - BmpHeaderLen, 1, bmpIn);
    }
}

void bmpDataFrame(FILE *bmpData) {
    int i, j, stride = ((bmpIHeader.bIWidth * bmpIHeader.bIBitCount + 31) >> 5) << 2;
    unsigned char *pix = (unsigned char *) malloc(stride);
    switch (bmpIHeader.bIBitCount) {
        case 8:
            for (j = 0; j < bmpIHeader.bIHeight; j++) {
                fread(pix, 1, stride, bmpData);
                for (i = 0; i < bmpIHeader.bIWidth; i++) {
                    gray[bmpIHeader.bIHeight - 1 - j][i] = pix[i];
                }
            }
            break;
        case 24:
            for (j = 0; j < bmpIHeader.bIHeight; j++) {
                fread(pix, 1, stride, bmpData);

                for (i = 0; i < bmpIHeader.bIWidth; i++) {
                    b[bmpIHeader.bIHeight - 1 - j][i] = pix[i * 3];
                    g[bmpIHeader.bIHeight - 1 - j][i] = pix[i * 3 + 1];
                    r[bmpIHeader.bIHeight - 1 - j][i] = pix[i * 3 + 2];
                }
            }
            break;
        default:
            printf("Cannot process %u bitCount, please retry with other file", bmpIHeader.bIBitCount);
    }
}

void writeFile(FILE *bmpFileOut) {
    addHeader(bmpFileOut);

    bmpOutput(bmpFileOut);
}

void headerChange(int bitCount, int height, int width) {
    bmpIHeader.bIBitCount = bitCount;
    bmpIHeader.bIHeight = height;
    bmpIHeader.bIWidth = width;
    bmpIHeader.bISizeImage = (((width * bitCount + 31) >> 5) << 2) * height;
    switch (bitCount) {
        case 8:
            bmpFHeader.bFOffBits_1 = (BmpHeaderLen + BmpBoardLen) << 16 >> 16;
            bmpFHeader.bFOffBits_2 = (BmpHeaderLen + BmpBoardLen) >> 16;
            bmpFHeader.bFOffBits = (bmpFHeader.bFOffBits_2 << 16) + bmpFHeader.bFOffBits_1;

            bmpFHeader.bFSize_1 = (bmpIHeader.bISizeImage + BmpHeaderLen + BmpBoardLen) << 16 >> 16;
            bmpFHeader.bFSize_2 = (bmpIHeader.bISizeImage + BmpHeaderLen + BmpBoardLen) >> 16;
            bmpFHeader.bFSize = (bmpFHeader.bFSize_2 << 16) + bmpFHeader.bFSize_1;
            break;
        case 24:
            bmpFHeader.bFOffBits_1 = BmpHeaderLen;
            bmpFHeader.bFOffBits_2 = BmpHeaderLen >> 16;
            bmpFHeader.bFOffBits = (bmpFHeader.bFOffBits_2 << 16) + bmpFHeader.bFOffBits_1;

            bmpFHeader.bFSize_1 = (bmpIHeader.bISizeImage + BmpHeaderLen) << 16 >> 16;
            bmpFHeader.bFSize_2 = (bmpIHeader.bISizeImage + BmpHeaderLen) >> 16;
            bmpFHeader.bFSize = (bmpFHeader.bFSize_2 << 16) + bmpFHeader.bFSize_1;
            break;
        default:
            printf("Cannot process %u bitCount, please retry with other file", bmpIHeader.bIBitCount);
    }
}

void addHeader(FILE *bmpFileOut) {
    fwrite(&bmpFHeader, BmpFHeaderLen, 1, bmpFileOut);
    fwrite(&bmpIHeader, BmpIHeaderLen, 1, bmpFileOut);

    if (bmpIHeader.bIBitCount == 8 && otherInfo == NULL) {
        FILE *bmpBoard = fopen(BmpBoard, "rb");
        otherInfo = (unsigned char *) malloc(BmpBoardLen);
        fread(otherInfo, 1, BmpBoardLen, bmpBoard);
        fwrite(otherInfo, BmpBoardLen, 1, bmpFileOut);
    } else {
        fwrite(otherInfo, bmpFHeader.bFOffBits - BmpHeaderLen, 1, bmpFileOut);
    }
}

void bmpOutput(FILE *bmpFileOut) {
    int i, j, stride = ((bmpIHeader.bIWidth * bmpIHeader.bIBitCount + 31) >> 5) << 2;
    unsigned char *pixout = (unsigned char *) malloc(stride);
    memset(pixout, 0, stride);

    switch (bmpIHeader.bIBitCount) {
        case 8:
            for (j = 0; j < bmpIHeader.bIHeight; j++) {
                for (i = 0; i < bmpIHeader.bIWidth; i++) {
                    pixout[i] = gray[bmpIHeader.bIHeight - 1 - j][i];
                }
                fwrite(pixout, 1, stride, bmpFileOut);
            }
            break;
        case 24:
            for (j = 0; j < bmpIHeader.bIHeight; j++) {
                for (i = 0; i < bmpIHeader.bIWidth; i++) {
                    pixout[i * 3] = b[bmpIHeader.bIHeight - 1 - j][i];
                    pixout[i * 3 + 1] = g[bmpIHeader.bIHeight - 1 - j][i];
                    pixout[i * 3 + 2] = r[bmpIHeader.bIHeight - 1 - j][i];
                }
                fwrite(pixout, 1, stride, bmpFileOut);
            }
            break;
        default:
            printf("Cannot process %u bitCount, please retry with other file", bmpIHeader.bIBitCount);
    }
}
