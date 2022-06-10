//
// Created by SNAPDRAGON_11800H on 3/11/2022.
//

#ifndef IMGPROCESS_IN_C_BMPIO_H
#define IMGPROCESS_IN_C_BMPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../settings/settings.h"

void readBMPFile(FILE *bmpFileIn);

void writeBMPFile(FILE *bmpFileOut);

void bmpFHeaderRead(FILE *bmpFileIn);

void bmpIHeaderRead(FILE *bmpFileIn);

void skipPixelInfo();

void bmpDataFrame(FILE *bmpFileIn);

void headerChange(int bitCount, int height, int width);

void addHeader(FILE *bmpFileOut);

void bmpOutput(FILE *bmpFileOut);


#endif //IMGPROCESS_IN_C_BMPIO_H
