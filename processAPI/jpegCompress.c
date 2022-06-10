//
// Created by SNAPDRAGON_11800H on 6/11/2022.
//

#include "jpegCompress.h"

void jpeg_dct(float *d0, float *d1, float *d2, float *d3, float *d4, float *d5, float *d6, float *d7) {
    float t0 = (*d0) + (*d7);
    float t7 = (*d0) - (*d7);
    float t1 = (*d1) + (*d6);
    float t6 = (*d1) - (*d6);
    float t2 = (*d2) + (*d5);
    float t5 = (*d2) - (*d5);
    float t3 = (*d3) + (*d4);
    float t4 = (*d3) - (*d4);

    // even part
    float t10 = t0 + t3;
    float t13 = t0 - t3;
    float t11 = t1 + t2;
    float t12 = t1 - t2;

    (*d0) = t10 + t11;
    (*d4) = t10 - t11;

    float z1 = (t12 + t13) * 0.707106781f;
    (*d2) = t13 + z1;
    (*d6) = t13 - z1;

    // odd part
    t10 = t4 + t5;
    t11 = t5 + t6;
    t12 = t6 + t7;

    float z5 = (t10 - t12) * 0.382683433f;
    float z2 = t10 * 0.541196100f + z5;
    float z4 = t12 * 1.306562965f + z5;
    float z3 = t11 * 0.707106781f;

    float z11 = t7 + z3;
    float z13 = t7 - z3;

    (*d5) = z13 + z2;
    (*d3) = z13 - z2;
    (*d1) = z11 + z4;
    (*d7) = z11 - z4;
}


int jpeg_process_dct_unit(unsigned int *bit_buffer, unsigned int *num_of_bits, float *dct_unit, float *fast_dct_table,
                          int dc_value, const unsigned short huffman_dc[256][2],
                          const unsigned short huffman_ac[256][2], FILE *fp, unsigned char step) {
    unsigned int i;

    const unsigned short eob[2] = {huffman_ac[0x00][0], huffman_ac[0x00][1]};
    const unsigned short m16_zeroes[2] = {huffman_ac[0xF0][0], huffman_ac[0xF0][1]};

    // dct rows
    unsigned int data_offset;
    for (data_offset = 0; data_offset < 64; data_offset += 8) {
        jpeg_dct(dct_unit + data_offset, dct_unit + data_offset + 1,
                 dct_unit + data_offset + 2, dct_unit + data_offset + 3,
                 dct_unit + data_offset + 4, dct_unit + data_offset + 5,
                 dct_unit + data_offset + 6, dct_unit + data_offset + 7);
    }

    // dct colums
    for (data_offset = 0; data_offset < 8; data_offset++) {
        jpeg_dct(dct_unit + data_offset, dct_unit + data_offset + 8,
                 dct_unit + data_offset + 16, dct_unit + data_offset + 24,
                 dct_unit + data_offset + 32, dct_unit + data_offset + 40,
                 dct_unit + data_offset + 48, dct_unit + data_offset + 56);
    }

#ifdef EXPERIMENT_NUM_1
    for (i=0; i<64; i++) {
        // horizontal
        if (i % 8 > step) {
            dct_unit[i] = 0;
        }
    }
#endif
#ifdef EXPERIMENT_NUM_2
    for (i=0; i<64; i++) {
        // vertical
        if (i / 8 > step) {
            dct_unit[i] = 0;
        }
    }
#endif
#ifdef EXPERIMENT_NUM_3
    for (i = 0; i < 64; i++) {
        if (i % 8 > step) {
            dct_unit[i] = 0;
        }
        if (i / 8 > step) {
            dct_unit[i] = 0;
        }
    }
#endif

    // quantize/descale/zigzag the coefficients
    int unit_out[64];
    for (i = 0; i < 64; i++) {
        float val = dct_unit[i] * fast_dct_table[i];
        unit_out[zigzag_index[i]] = roundf(val);
    }

    // encode dc value
    int diff = unit_out[0] - dc_value;
    if (diff == 0) {
        jpeg_write_bits(bit_buffer, num_of_bits, huffman_dc[0], fp);
    } else {
        unsigned short bits[2];
        jpeg_calc_bits(diff, bits);
        jpeg_write_bits(bit_buffer, num_of_bits, huffman_dc[bits[1]], fp);
        jpeg_write_bits(bit_buffer, num_of_bits, bits, fp);
    }

    // encode ac
    unsigned int end_0_pos = 63;
    while ((end_0_pos > 0) && (unit_out[end_0_pos] == 0)) {
        end_0_pos--;
    }

    // end_0_pos = first element in reverse order != 0
    if (end_0_pos == 0) {
        jpeg_write_bits(bit_buffer, num_of_bits, eob, fp);
        return unit_out[0];
    }

    for (i = 1; i <= end_0_pos; i++) {
        unsigned int start_pos = i;
        while ((unit_out[i] == 0) && (i <= end_0_pos)) {
            i++;
        }

        unsigned int nr_zeroes = i - start_pos;
        if (nr_zeroes >= 16) {
            unsigned int lng = nr_zeroes >> 4, nr_marker;
            for (nr_marker = 1; nr_marker <= lng; nr_marker++) {
                jpeg_write_bits(bit_buffer, num_of_bits, m16_zeroes, fp);
            }
            nr_zeroes &= 15;
        }

        unsigned short bits[2];
        jpeg_calc_bits(unit_out[i], bits);
        jpeg_write_bits(bit_buffer, num_of_bits,
                        huffman_ac[(nr_zeroes << 4) + bits[1]], fp);
        jpeg_write_bits(bit_buffer, num_of_bits, bits, fp);
    }

    if (end_0_pos != 63) {
        jpeg_write_bits(bit_buffer, num_of_bits, eob, fp);
    }

    return unit_out[0];
}

