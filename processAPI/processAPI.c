//
// Created by snapdragon_manjaro21 on 2022/3/10.
//

#include "processAPI.h"

void linearTransform8(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX],
                      unsigned char dat[][MAX]) {
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

void dct2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {
    dctinit(height, width);
    int2fl(dat, dat_fl, height, width);
    dct(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}

void idct2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {
    idct(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}


void fft2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {
    int2fl(dat, dat_fl, height, width);

    fft(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}

void fft2_freq(unsigned char dat[][MAX], float **dat_fl, int height, int width, int multTimes) {
    fftinit(height, width);
    int2fl(dat, dat_fl, height, width);

    fft(dat_fl, height, width);

    mod(modulus, dat_fl, height, width);
    fftshift(modulus, height, width);

    mult(modulus, multTimes, height, width);
    center(modulus, dat_fl, height, width);

    fl2int(dat_fl, dat, height, width);
}

void ifft2(unsigned char dat[][MAX], float **dat_fl, int height, int width) {
    ifft(dat_fl, height, width);
    fl2int(dat_fl, dat, height, width);
}

int jpeg_exp_write(FILE *Out, int quality_o, int step) { //step means experiment_num
    unsigned int row, col, x, y;

    // actual quality value
    int quality = (quality_o < 50) ? (5000 / quality_o) : (200 - quality_o * 2);

    gen_tables(quality);

    addJpegHeader(Out);

    // encode 8x8 macro blocks
    unsigned int bit_buffer = 0, num_of_bits = 0, pos;
    int dc_y = 0, dc_cb = 0, dc_cr = 0; // dc component of Y
    float dct_unit_y[64];
    float dct_unit_u[64];
    float dct_unit_v[64];

    for (y = 0; y < bmpIHeader.bIHeight; y += 8) { // for blocks
        for (x = 0; x < bmpIHeader.bIWidth; x += 8) {

            // rgb to ycbcr
            for (row = y, pos = 0; row < y + 8; row++) {
                for (col = x; col < x + 8; col++, pos++) {
                    float rt = r[row][col], gt = g[row][col], bt = b[row][col];
                    dct_unit_y[pos] = 0.29900f * rt + 0.58700f * gt + 0.11400f * bt - 128;
                    dct_unit_u[pos] = -0.16870f * rt - 0.33130f * gt + 0.50000f * bt;
                    dct_unit_v[pos] = 0.50000f * rt - 0.41870f * gt - 0.08130f * bt;
                }
            }

            // compute dct unit, encode and write
            dc_y = jpeg_process_dct_unit(&bit_buffer, &num_of_bits, dct_unit_y, fast_dct_table_y,
                                         dc_y, huffman_dc_y, huffman_ac_y, Out, step);
            dc_cb = jpeg_process_dct_unit(&bit_buffer, &num_of_bits, dct_unit_u, fast_dct_table_cbcr,
                                         dc_cb, huffman_dc_uv, huffman_ac_uv, Out, step);
            dc_cr = jpeg_process_dct_unit(&bit_buffer, &num_of_bits, dct_unit_v, fast_dct_table_cbcr,
                                         dc_cr, huffman_dc_uv, huffman_ac_uv, Out, step);
        }
    }

    // bit alignment of eoi marker
    const unsigned short filler[] = {0x7F, 0x07};
    jpeg_write_bits(&bit_buffer, &num_of_bits, filler, Out);

    fillJpegLast(Out);
}
