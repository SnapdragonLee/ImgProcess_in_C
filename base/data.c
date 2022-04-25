//
// Created by SNAPDRAGON_11800H on 4/25/2022.
//

#include "data.h"

void int2fl(unsigned char in[][MAX], float **out, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            out[i][j] = in[i][j];
        }
    }
}

void fl2int(float **in, unsigned char out[][MAX], int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            out[i][j] = (unsigned char) in[i][j];
        }
    }
}
