
//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#ifndef INC_2022_3_10_HISTOGRAM_EQUALIZATION_IMAGEAPI_H
#define INC_2022_3_10_HISTOGRAM_EQUALIZATION_IMAGEAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BITMAPFILEHEADERLENGTH 14   // The bmp FileHeader length is 14
#define BM 19778                    // The ASCII code for BM

#define MAXSIZE 10000

typedef struct bmpFileHeader
{
    unsigned short bFType;        // BM字符串,0x4d42,19778
    unsigned int   bFSize;        // 文件大小 以字节为单位(2-5字节)
    unsigned short bFReserved1;   // 保留，必须设置为0 (6-7字节)
    unsigned short bFReserved2;   // 保留，必须设置为0 (8-9字节)
    unsigned int   bFOffBits;     // 从文件头到像素数据的偏移  (10-13字节)
} bmpFileHeader;

typedef struct bmpInfoHeader
{
    unsigned int    bISize;          // 此结构体的大小 (14-17字节)
    long            bIWidth;         // 图像的宽  (18-21字节)
    long            bIHeight;        // 图像的高  (22-25字节)
    unsigned short  bIPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short  bIBitCount;      // 一像素所占的位数，一般为24   (28-29字节)
    unsigned int    bICompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
    unsigned int    bISizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
    long            bIXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
    long            bIYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    unsigned int    bIClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。 (46-49字节)
    unsigned int    bIClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} bmpInfoHeader;

void RGBtoYUV();

void YUVtoGray();

void YUVtoRGB();

void ChangeTheLuminance();


//================My file 1 end=============//

//================My file 2 start=============//
void YUVtoBinarization();

#define elementline 3

void Dilation_Lining(char dowhat);

void Erosion();

void Opening_Closing(char dowhat);

void BetweenTwoOperationProcessing();
//================My file 2 end=============//

//================My file 3 end=============//
void VisibilityEnhancement();

void HistogramEqualization_r();

void HistogramEqualization_g();

void HistogramEqualization_b();

void HistogramEqualization_Y();
//================My file 3 end=============//

//================My file 4 start==========//
void Scaling(float ratioX, float ratioY);

void Rotation(double angle);

double Gaussian(int radial, double lamda);

void RBF_Gaussian(unsigned char temple[], int x, int y, int paramenter);

void Shearing(float dx, float dy);

void mirror(char character);

void translation(int sizeX, int sizeY, int sizeX1, int sizeY1, int x, int y);

void translation1(int sizeX, int sizeY, int sizeX1, int sizeY1, int x, int y);
//================My file 4 end=============//

//================My file 5 start==========//
void meanFiltering(int paramenter);

void MeanFilter(unsigned char temple[], int x, int y, int paramenter);

unsigned char Laplacian_filter_Y(int x, int y);

void LaplacianFilter_Y(double paramenter);

void Meanfilter_Y(int paramenter);
//================My file 5 end=============//

//================My file 6 start==========//
void BilateralFilter(int paramenter, double lamda_range, double lamda_color);

void RBF_Bilateral(unsigned char temple[], int x, int y, int paramenter, double lamda_range, double lamda_color);
//================My file 6 end=============//

//================Someone's work===========//
/* Test the file is bmp file or not */
void bmpFHeaderRead(FILE *fpbmp);

/* To get the OffSet of header to data part */
void bmpIHeaderRead(FILE *fpbmp);

/* To get the width and height of the bmp file */
void BmpWidthHeight(FILE *fpbmp);

//get r,g,b data
void bmpDataPart(FILE *fpbmp);

// output data to corresponding txt file
void bmpoutput(FILE *fpout);
//===========Someone's work end===========//

//===========Rewrite File ================//
void Initialization();

//open a picture
void readfile(FILE * bmpFileIn);

//creat a picture
FILE *writefile(char filename[]);

//add head to file
void addHeadertofile(FILE *input, FILE *output);
//===========Rewrite File End================//

//============TEST FUNCTION=================//
void testInputYUV();

void testOutputYUV();

void testRGB();

void NoneProcess();
//=============TEST FUNCTION END============//


#endif //INC_2022_3_10_HISTOGRAM_EQUALIZATION_IMAGEAPI_H
