//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#include "processAPI.h"

void
linearTransform8(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char dat[][MAX]) {
    RGB2Gray(r, g, b, dat);
    LinearTransform(dat);
}

void linearTransform24(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX]) {
    LinearTransform(r);
    LinearTransform(g);
    LinearTransform(b);
}

void histogramEqualization24(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX]) {
    HistogramEqualization(r);
    HistogramEqualization(g);
    HistogramEqualization(b);
}

void histogramEqualization8(unsigned char gray[][MAX]) {
    HistogramEqualization(gray);
}

void dct2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {
    dctinit(height, width);
    int2fl(dat, dat_fl, height, width);
    dct(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}

void idct2(unsigned char dat[][MAX], float **dat_fl, int height, int width){
    idct(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}


void fft2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {
    int2fl(dat, dat_fl, height, width);

    fft(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}

void fft2_freq(unsigned char dat[][MAX], float **dat_fl, int height, int width, int multTimes) {

    fftinit(height, width);
    int2fl(dat, dat_fl, height, width);

    fft(dat_fl, height, width);

    mod(modulus, dat_fl, height, width);
    fftshift(modulus, height, width);

    mult(modulus, multTimes, height, width);
    center(modulus, dat_fl, height, width);

    fl2int(dat_fl, dat, height, width);
}

void ifft2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {

    ifft(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}