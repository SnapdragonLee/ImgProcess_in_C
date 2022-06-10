//
// Created by SNAPDRAGON_11800H on 6/6/2022.
//

#ifndef IMGPROCESS_IN_C_JPEGIO_H
#define IMGPROCESS_IN_C_JPEGIO_H

#endif //IMGPROCESS_IN_C_JPEGIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../settings/settings.h"


extern const unsigned int quantization_table_y[64], quantization_table_uv[64];

extern unsigned char y_table[64], cbcr_table[64];
extern float fast_dct_table_y[64], fast_dct_table_cbcr[64];

extern const unsigned char zigzag_index[64],
        std_dc_luminance_nrcodes[17], std_dc_luminance_values[12], std_ac_luminance_nrcodes[17], std_ac_luminance_values[162],
        std_dc_chrominance_nrcodes[17], std_dc_chrominance_values[12], std_ac_chrominance_nrcodes[17], std_ac_chrominance_values[162];

extern const unsigned short huffman_dc_y[256][2], huffman_ac_y[256][2], huffman_dc_uv[256][2], huffman_ac_uv[256][2];

extern const float aasf[8];

void gen_tables(int quality);

void addJpegHeader(FILE *jpegIOBuf);

void fillJpegLast(FILE *jpegIOBuf);

void jpeg_write_bits(unsigned int *bit_buffer, unsigned int *num_of_bits, const unsigned short *bs, FILE *fp);

void jpeg_calc_bits(int val, unsigned short bits[2]);