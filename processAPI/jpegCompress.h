//
// Created by SNAPDRAGON_11800H on 6/11/2022.
//

#ifndef IMGPROCESS_IN_C_JPEGCOMPRESS_H
#define IMGPROCESS_IN_C_JPEGCOMPRESS_H

#endif //IMGPROCESS_IN_C_JPEGCOMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../settings/settings.h"
#include "../fileIO/jpegIO.h"

void jpeg_dct(float *d0, float *d1, float *d2, float *d3, float *d4, float *d5, float *d6, float *d7);

int jpeg_process_dct_unit(unsigned int *bit_buffer, unsigned int *num_of_bits, float *dct_unit, float *fast_dct_table,
                          int dc_value, const unsigned short huffman_dc[256][2],
                          const unsigned short huffman_ac[256][2], FILE *fp, unsigned char step);