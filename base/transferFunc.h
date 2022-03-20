//
// Created by SNAPDRAGON_11800H on 3/18/2022.
//

#ifndef IMGPROCESS_IN_C_TRANSFERFUNC_H
#define IMGPROCESS_IN_C_TRANSFERFUNC_H

#include <stdio.h>
#include <math.h>

#include "../settings/settings.h"

void RGB2Gray(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char gray[][MAX]);

void RGB2YUV(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char YuvY[][MAX],
             unsigned char U[][MAX], unsigned char V[][MAX]);

void YUV2Gray(unsigned char YuvY[][MAX], unsigned char U[][MAX], unsigned char V[][MAX], unsigned char r[][MAX],
              unsigned char g[][MAX], unsigned char b[][MAX]);

void YUV2RGB(unsigned char YuvY[][MAX], unsigned char U[][MAX], unsigned char V[][MAX], unsigned char r[][MAX],
             unsigned char g[][MAX], unsigned char b[][MAX]);

void YUV2Binarization(unsigned char YuvY[][MAX], unsigned char U[][MAX], unsigned char V[][MAX], unsigned char r[][MAX],
                      unsigned char g[][MAX], unsigned char b[][MAX]);

#endif //IMGPROCESS_IN_C_TRANSFERFUNC_H
