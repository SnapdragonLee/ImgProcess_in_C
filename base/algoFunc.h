//
// Created by snapdragon_manjaro21 on 2022/3/17.
//

#ifndef IMGPROCESS_IN_C_ALGOFUNC_H
#define IMGPROCESS_IN_C_ALGOFUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "../settings/settings.h"

void HistogramEqualization(unsigned char dat[][MAX]);

void ChangeTheLuminance();

void Dilation_Lining(char dowhat);

void Erosion();

void Opening_Closing(char dowhat);

void VisibilityEnhancement();

void Scaling(float ratioX, float ratioY);

void Rotation(double angle);

double Gaussian(int radial, double lamda);

void RBF_Gaussian(unsigned char temple[], int x, int y, int paramenter);

void Shearing(float dx, float dy);

void mirror(char character);

void translation(int sizeX, int sizeY, int sizeX1, int sizeY1, int x, int y);

void translation1(int sizeX, int sizeY, int sizeX1, int sizeY1, int x, int y);

void meanFiltering(int paramenter);

void MeanFilter(unsigned char temple[], int x, int y, int paramenter);

unsigned char Laplacian_filter_Y(int x, int y);

void LaplacianFilter_Y(double paramenter);

void Meanfilter_Y(int paramenter);

void BilateralFilter(int paramenter, double lamda_range, double lamda_color);

void RBF_Bilateral(unsigned char temple[], int x, int y, int paramenter, double lamda_range, double lamda_color);

void testInputYUV();

void testOutputYUV();

void testRGB();

#endif //IMGPROCESS_IN_C_ALGOFUNC_H
