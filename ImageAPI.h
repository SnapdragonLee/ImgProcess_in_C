
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


//================My file 1 start=============//
#define MAXSIZE 10000

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
void bmpFileTest(FILE *fpbmp);

/* To get the OffSet of header to data part */
void bmpHeaderPartLength(FILE *fpbmp);

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
void openfile(FILE * bmpFileIn);

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
