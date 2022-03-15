//
// Created by SNAPDRAGON_11800H on 3/11/2022.
//

#ifndef IMGPROCESS_IN_C_BMPIO_H
#define IMGPROCESS_IN_C_BMPIO_H

#define BmpFHeaderLen 14
#define BmpIHeaderLen 40

#define BM 19778                  // The ASCII code for BM

typedef struct bmpFileHeader {
    unsigned short bFType;        // File type (0x4d42 19778)
    unsigned short bFSize_1;      // File size in bytes _1 (2-3 Bytes)
    unsigned short bFSize_2;      // File size in bytes _2 (4-5 Bytes)
    unsigned short bFReserved1;   // Reserved (6-7 Bytes)
    unsigned short bFReserved2;   // Reserved (8-9 Bytes)
    unsigned short bFOffBits_1;   // Offset to image data _1  (10-11 Bytes)
    unsigned short bFOffBits_2;   // Offset to image data _2  (12-13 Bytes)
    unsigned int bFSize;          // File size
    unsigned int bFOffBits;       // Offset to image data
} bmpFileHeader;

typedef struct bmpInfoHeader {
    unsigned int bISize;          // Info header size (14-17 Bytes)
    long bIWidth;                 // Width  (18-21 Bytes)
    long bIHeight;                // Height  (22-25 Bytes)
    unsigned short bIPlanes;      // Number of colour planes (26-27 Bytes)
    unsigned short bIBitCount;    // Bits per pixel (28-29 Bytes)
    unsigned int bICompression;   // Compression type (30-33 Bytes)
    unsigned int bISizeImage;     // Image size (34-37 Bytes)
    long bIXPelsPerMeter;         // XPixels per meter (38-41 Bytes)
    long bIYPelsPerMeter;         // YPixels per meter (42-45 Bytes)
    unsigned int bIClrUsed;       // Number of colours (46-49 Bytes)
    unsigned int bIClrImportant;  // Important colours (50-53 Bytes)
} bmpInfoHeader;

typedef struct bmpPixelInfo {
    unsigned char rgbBlue;     // B  (0-255)
    unsigned char rgbGreen;    // G  (0-255)
    unsigned char rgbRed;      // R  (0-255)
    unsigned char rgbReserved; // Reserved
} bmpPixelInfo;

void readFile(FILE *bmpFileIn);

void bmpFHeaderRead(FILE *bmpFileIn);

void bmpIHeaderRead(FILE *bmpFileIn);

void bmpDataPart(FILE *bmpFileIn);

void writeFile(FILE *bmpFileOut);

void bmpOutput(FILE *bmpFileOut);

void addHeader(FILE *bmpFileOut);





#endif //IMGPROCESS_IN_C_BMPIO_H
