//
// Created by SNAPDRAGON_11800H on 3/15/2022.
//

#ifndef IMGPROCESS_IN_C_SETTINGS_H
#define IMGPROCESS_IN_C_SETTINGS_H

#define MAX 1024
#define elementline 3

#define BmpFHeaderLen 14
#define BmpIHeaderLen 40
#define BmpHeaderLen (BmpFHeaderLen + BmpIHeaderLen)

#define BmpBoardLen 1024

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
    int bIWidth;                  // Width  (18-21 Bytes)
    int bIHeight;                 // Height  (22-25 Bytes)
    unsigned short bIPlanes;      // Number of colour planes (26-27 Bytes)
    unsigned short bIBitCount;    // Bits per pixel (28-29 Bytes)
    unsigned int bICompression;   // Compression type (30-33 Bytes)
    unsigned int bISizeImage;     // Image size (34-37 Bytes)
    int bIXPelsPerMeter;          // XPixels per meter (38-41 Bytes)
    int bIYPelsPerMeter;          // YPixels per meter (42-45 Bytes)
    unsigned int bIClrUsed;       // Number of colours (46-49 Bytes)
    unsigned int bIClrImportant;  // Important colours (50-53 Bytes)
} bmpInfoHeader;

typedef struct bmpPixelInfo {
    unsigned char rgbBlue;        // B  (0-255)
    unsigned char rgbGreen;       // G  (0-255)
    unsigned char rgbRed;         // R  (0-255)
    unsigned char rgbReserved;    // Reserved
} bmpPixelInfo;

extern bmpFileHeader bmpFHeader;
extern bmpInfoHeader bmpIHeader;
extern bmpPixelInfo bmpPx;

extern unsigned char YCbCrY[MAX][MAX];
extern unsigned char Cb[MAX][MAX];
extern unsigned char Cr[MAX][MAX];

extern unsigned char YuvY[MAX][MAX];
extern unsigned char U[MAX][MAX];
extern unsigned char V[MAX][MAX];

extern unsigned char r[MAX][MAX], output_r[MAX][MAX];
extern unsigned char g[MAX][MAX], output_g[MAX][MAX];
extern unsigned char b[MAX][MAX], output_b[MAX][MAX];
extern unsigned char records[MAX][MAX];

extern unsigned char gray[MAX][MAX];


extern char *SourceFile_Path, *OutputFile_Path;
extern const char *OutputFile_Down;
extern const char *BmpBoard;

#endif //IMGPROCESS_IN_C_SETTINGS_H
