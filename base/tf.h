//
// Created by snapdragon_manjaro21 on 4/20/2022.
//

#ifndef IMGPROCESS_IN_C_TF_H
#define IMGPROCESS_IN_C_TF_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../settings/settings.h"

#define FFT   1
#define IFFT -1

#define swap(x, y) do {typeof(x) _x = x; \
                       typeof(y) _y = y; \
                       x = _y; y = _x;   \
                      } while(0)
float **dat_fl;
float *dat_fl_l;

float **dat_fl, **matimg, **modulus;
float *mat_line, *mod_line;

float **matdct;
float *dct_line;

void fftinit(int height, int width);

void dctinit(int height, int width);

void dct(float **in, int height, int width);

void idct(float **in, int height, int width);

void fft(float **matreal, int height, int width);

void ifft(float **matreal, int height, int width);

void mod(float **modout, float **matreal, int lgth, int wdth);

void fftshift(float **mat, int height, int width);

void mult(float **mat, int coef, int height, int width);

void center(float **in, float **out, int height, int width);

void fourrier(float data[], int lw[], int sign);

#endif //IMGPROCESS_IN_C_TF_H
