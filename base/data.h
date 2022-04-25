//
// Created by SNAPDRAGON_11800H on 4/25/2022.
//

#ifndef IMGPROCESS_IN_C_DATA_H
#define IMGPROCESS_IN_C_DATA_H

#include <stdio.h>

#include "../settings/settings.h"

void int2fl(unsigned char in[][MAX], float **out, int height, int width);

void fl2int(float **in, unsigned char out[][MAX], int height, int width);

#endif //IMGPROCESS_IN_C_DATA_H
