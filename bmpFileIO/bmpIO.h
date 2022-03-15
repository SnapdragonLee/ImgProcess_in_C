//
// Created by SNAPDRAGON_11800H on 3/11/2022.
//

#ifndef IMGPROCESS_IN_C_BMPIO_H
#define IMGPROCESS_IN_C_BMPIO_H

#define BmpFHeaderLen 14
#define BmpIHeaderLen 40

#define BM 19778                  // The ASCII code for BM

typedef struct bmpFileHeader {
    unsigned short bFType;        // BM字符串 (0x4d42 19778)
    unsigned short bFSize_1;      // 文件大小段_1 (2-3 Bytes)
    unsigned short bFSize_2;      // 文件大小段_2 (4-5 Bytes)
    unsigned short bFReserved1;   // 保留，必须设置为0 (6-7 Bytes)
    unsigned short bFReserved2;   // 保留，必须设置为0 (8-9 Bytes)
    unsigned short bFOffBits_1;   // 从文件头到像素数据的偏移段1  (10-11 Bytes)
    unsigned short bFOffBits_2;   // 从文件头到像素数据的偏移段2  (12-13 Bytes)
    unsigned int bFSize;          // 文件大小段 合成
    unsigned int bFOffBits;       // 从文件头到像素数据的偏移段 合成
} bmpFileHeader;

typedef struct bmpInfoHeader {
    unsigned int bISize;          // 此结构体的大小 (14-17 Bytes)
    long bIWidth;                 // 图像的宽  (18-21 Bytes)
    long bIHeight;                // 图像的高  (22-25 Bytes)
    unsigned short bIPlanes;      // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27 Bytes)
    unsigned short bIBitCount;    // 一像素所占的位数，一般为24   (28-29 Bytes)
    unsigned int bICompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33 Bytes)
    unsigned int bISizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37 Bytes)
    long bIXPelsPerMeter;         // 说明水平分辨率，用象素/米表示。一般为0 (38-41 Bytes)
    long bIYPelsPerMeter;         // 说明垂直分辨率，用象素/米表示。一般为0 (42-45 Bytes)
    unsigned int bIClrUsed;       // 说明位图实际使用的彩色表中的颜色索
    // 引数（设为0的话，则说明使用所有调色板项）。 (46-49 Bytes)
    unsigned int bIClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53 Bytes)
} bmpInfoHeader;

typedef struct bmpPixelInfo {
    unsigned char rgbBlue;   //该颜色的蓝色分量  (值范围为0-255)
    unsigned char rgbGreen;  //该颜色的绿色分量  (值范围为0-255)
    unsigned char rgbRed;    //该颜色的红色分量  (值范围为0-255)
    unsigned char rgbReserved;// 保留，必须为0
} bmpPixelInfo;

void readFile(FILE *bmpFileIn);

void bmpFHeaderRead(FILE *bmpFileIn);

/* To get the OffSet of header to data part */
void bmpIHeaderRead(FILE *bmpFileIn);

//get r,g,b data
void bmpDataPart(FILE *bmpFileIn);

// output data to corresponding txt file
void bmpOutput(FILE *bmpFileOut);

void addHeader(FILE *bmpFileOut);

void writeFile(FILE *bmpFileOut);





#endif //IMGPROCESS_IN_C_BMPIO_H
