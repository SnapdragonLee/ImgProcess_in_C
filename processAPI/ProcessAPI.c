//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#include "ProcessAPI.h"
#include "../settings/settings.h"

void linearTransform(unsigned char dat[][MAX]) {
    LinearTransform(dat);
}

void histogramEqualization24(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX]) {
    HistogramEqualization(r);
    HistogramEqualization(g);
    HistogramEqualization(b);
}

void histogramEqualization8(unsigned char gray[][MAX]) {
    if (bmpIHeader.bIBitCount == 24) {
        RGB2Gray();
    }
    HistogramEqualization(gray);
}