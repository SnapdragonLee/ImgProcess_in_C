//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#ifndef IMGPROCESS_IN_C_PROCESSAPI_H
#define IMGPROCESS_IN_C_PROCESSAPI_H

#include "../base/algoFunc.h"
#include "../base/transferFunc.h"
#include "../base/tf.h"
#include "../base/data.h"
#include "../fileIO/jpegIO.h"
#include "jpegCompress.h"

void linearTransform8(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char dat[][MAX]);

void linearTransform24(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX]);

void histogramEqualization24(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX]);

void histogramEqualization8(unsigned char gray[][MAX]);

void dct2(unsigned char dat[][MAX], float **dat_fl, int height, int width);

void idct2(unsigned char dat[][MAX], float **dat_fl, int height, int width);

void fft2(unsigned char dat[][MAX], float **dat_fl, int height, int width);

void fft2_freq(unsigned char dat[][MAX], float **dat_fl, int height, int width, int multTimes);

void ifft2(unsigned char dat[][MAX], float **dat_fl, int height, int width);

int jpeg_exp_write(FILE *Out, int quality_o, int step);

#endif //IMGPROCESS_IN_C_PROCESSAPI_H
