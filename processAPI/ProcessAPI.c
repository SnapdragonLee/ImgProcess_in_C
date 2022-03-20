//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#include "ProcessAPI.h"
#include "../settings/settings.h"

void linearTransform8(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char dat[][MAX]) {
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