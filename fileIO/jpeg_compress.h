//
// Created by SNAPDRAGON_11800H on 6/6/2022.
//

#ifndef IMGPROCESS_IN_C_JPEG_COMPRESS_H
#define IMGPROCESS_IN_C_JPEG_COMPRESS_H

const unsigned int quantization_table_y[], quantization_table_uv[];
const unsigned char zigzag_index[], std_dc_luminance_nrcodes[], std_dc_luminance_values[], std_ac_luminance_nrcodes[],
        std_ac_luminance_values[], std_dc_chrominance_nrcodes[], std_dc_chrominance_values[], std_ac_chrominance_nrcodes[],
        std_ac_chrominance_values[];

const unsigned short huffman_dc_y[256][2], huffman_ac_y[256][2], huffman_dc_uv[256][2], huffman_ac_uv[256][2];

const float aasf[];

#endif //IMGPROCESS_IN_C_JPEG_COMPRESS_H
