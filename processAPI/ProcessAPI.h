//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#ifndef IMGPROCESS_IN_C_PROCESSAPI_H
#define IMGPROCESS_IN_C_PROCESSAPI_H

#include "../base/algoFunc.h"

#include "../base/transferFunc.h"

void linearTransform(unsigned char dat[][MAX]);

void histogramEqualization24(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX]);

void histogramEqualization8(unsigned char gray[][MAX]);

#endif //IMGPROCESS_IN_C_PROCESSAPI_H
