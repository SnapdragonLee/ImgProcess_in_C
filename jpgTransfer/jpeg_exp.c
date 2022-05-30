#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <endian.h>

#include "bmp.h"
#include "jpeg_exp.h"

#define JPEG_QUALITY_PERCENT 100

// to index zigzag scanning conveniently
static const uint8_t zigzag_index[] = {
        0, 1, 5, 6, 14, 15, 27, 28, 2, 4, 7, 13, 16, 26,
        29, 42, 3, 8, 12, 17, 25, 30, 41, 43, 9, 11, 18, 24,
        31, 40, 44, 53, 10, 19, 23, 32, 39, 45, 52, 54, 20,
        22, 33, 38, 46, 51, 55, 60, 21, 34, 37, 47, 50, 56,
        59, 61, 35, 36, 48, 49, 57, 58, 62, 63
};

// encoding constancs Y
static const uint8_t std_dc_luminance_nrcodes[] = {
        0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
};
static const uint8_t std_dc_luminance_values[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};
static const uint8_t std_ac_luminance_nrcodes[] = {
        0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d
};
static const uint8_t std_ac_luminance_values[] = {
        0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06,
        0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
        0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72,
        0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45,
        0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
        0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75,
        0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
        0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3,
        0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
        0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
        0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
        0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4,
        0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};
// encoding constancs UV
static const uint8_t std_dc_chrominance_nrcodes[] = {
        0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
};
static const uint8_t std_dc_chrominance_values[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};
static const uint8_t std_ac_chrominance_nrcodes[] = {
        0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77
};
static const unsigned char std_ac_chrominance_values[] = {
        0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41,
        0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
        0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1,
        0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
        0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44,
        0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74,
        0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a,
        0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
        0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
        0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4,
        0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};

// Huffman tables Y
static const uint16_t huffman_dc_y[256][2] = {
        {0,   2},
        {2,   3},
        {3,   3},
        {4,   3},
        {5,   3},
        {6,   3},
        {14,  4},
        {30,  5},
        {62,  6},
        {126, 7},
        {254, 8},
        {510, 9}
};

static const uint16_t huffman_ac_y[256][2] = {
        {10,    4},
        {0,     2},
        {1,     2},
        {4,     3},
        {11,    4},
        {26,    5},
        {120,   7},
        {248,   8},
        {1014,  10},
        {65410, 16},
        {65411, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {12,    4},
        {27,    5},
        {121,   7},
        {502,   9},
        {2038,  11},
        {65412, 16},
        {65413, 16},
        {65414, 16},
        {65415, 16},
        {65416, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {28,    5},
        {249,   8},
        {1015,  10},
        {4084,  12},
        {65417, 16},
        {65418, 16},
        {65419, 16},
        {65420, 16},
        {65421, 16},
        {65422, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {58,    6},
        {503,   9},
        {4085,  12},
        {65423, 16},
        {65424, 16},
        {65425, 16},
        {65426, 16},
        {65427, 16},
        {65428, 16},
        {65429, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {59,    6},
        {1016,  10},
        {65430, 16},
        {65431, 16},
        {65432, 16},
        {65433, 16},
        {65434, 16},
        {65435, 16},
        {65436, 16},
        {65437, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {122,   7},
        {2039,  11},
        {65438, 16},
        {65439, 16},
        {65440, 16},
        {65441, 16},
        {65442, 16},
        {65443, 16},
        {65444, 16},
        {65445, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {123,   7},
        {4086,  12},
        {65446, 16},
        {65447, 16},
        {65448, 16},
        {65449, 16},
        {65450, 16},
        {65451, 16},
        {65452, 16},
        {65453, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {250,   8},
        {4087,  12},
        {65454, 16},
        {65455, 16},
        {65456, 16},
        {65457, 16},
        {65458, 16},
        {65459, 16},
        {65460, 16},
        {65461, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {504,   9},
        {32704, 15},
        {65462, 16},
        {65463, 16},
        {65464, 16},
        {65465, 16},
        {65466, 16},
        {65467, 16},
        {65468, 16},
        {65469, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {505,   9},
        {65470, 16},
        {65471, 16},
        {65472, 16},
        {65473, 16},
        {65474, 16},
        {65475, 16},
        {65476, 16},
        {65477, 16},
        {65478, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {506,   9},
        {65479, 16},
        {65480, 16},
        {65481, 16},
        {65482, 16},
        {65483, 16},
        {65484, 16},
        {65485, 16},
        {65486, 16},
        {65487, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {1017,  10},
        {65488, 16},
        {65489, 16},
        {65490, 16},
        {65491, 16},
        {65492, 16},
        {65493, 16},
        {65494, 16},
        {65495, 16},
        {65496, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {1018,  10},
        {65497, 16},
        {65498, 16},
        {65499, 16},
        {65500, 16},
        {65501, 16},
        {65502, 16},
        {65503, 16},
        {65504, 16},
        {65505, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {2040,  11},
        {65506, 16},
        {65507, 16},
        {65508, 16},
        {65509, 16},
        {65510, 16},
        {65511, 16},
        {65512, 16},
        {65513, 16},
        {65514, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {65515, 16},
        {65516, 16},
        {65517, 16},
        {65518, 16},
        {65519, 16},
        {65520, 16},
        {65521, 16},
        {65522, 16},
        {65523, 16},
        {65524, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {2041,  11},
        {65525, 16},
        {65526, 16},
        {65527, 16},
        {65528, 16},
        {65529, 16},
        {65530, 16},
        {65531, 16},
        {65532, 16},
        {65533, 16},
        {65534, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0}
};
// Huffman tables UV
static const unsigned short huffman_dc_uv[256][2] = {
        {0,    2},
        {1,    2},
        {2,    2},
        {6,    3},
        {14,   4},
        {30,   5},
        {62,   6},
        {126,  7},
        {254,  8},
        {510,  9},
        {1022, 10},
        {2046, 11}
};
static const unsigned short huffman_ac_uv[256][2] = {
        {0,     2},
        {1,     2},
        {4,     3},
        {10,    4},
        {24,    5},
        {25,    5},
        {56,    6},
        {120,   7},
        {500,   9},
        {1014,  10},
        {4084,  12},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {11,    4},
        {57,    6},
        {246,   8},
        {501,   9},
        {2038,  11},
        {4085,  12},
        {65416, 16},
        {65417, 16},
        {65418, 16},
        {65419, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {26,    5},
        {247,   8},
        {1015,  10},
        {4086,  12},
        {32706, 15},
        {65420, 16},
        {65421, 16},
        {65422, 16},
        {65423, 16},
        {65424, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {27,    5},
        {248,   8},
        {1016,  10},
        {4087,  12},
        {65425, 16},
        {65426, 16},
        {65427, 16},
        {65428, 16},
        {65429, 16},
        {65430, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {58,    6},
        {502,   9},
        {65431, 16},
        {65432, 16},
        {65433, 16},
        {65434, 16},
        {65435, 16},
        {65436, 16},
        {65437, 16},
        {65438, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {59,    6},
        {1017,  10},
        {65439, 16},
        {65440, 16},
        {65441, 16},
        {65442, 16},
        {65443, 16},
        {65444, 16},
        {65445, 16},
        {65446, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {121,   7},
        {2039,  11},
        {65447, 16},
        {65448, 16},
        {65449, 16},
        {65450, 16},
        {65451, 16},
        {65452, 16},
        {65453, 16},
        {65454, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {122,   7},
        {2040,  11},
        {65455, 16},
        {65456, 16},
        {65457, 16},
        {65458, 16},
        {65459, 16},
        {65460, 16},
        {65461, 16},
        {65462, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {249,   8},
        {65463, 16},
        {65464, 16},
        {65465, 16},
        {65466, 16},
        {65467, 16},
        {65468, 16},
        {65469, 16},
        {65470, 16},
        {65471, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {503,   9},
        {65472, 16},
        {65473, 16},
        {65474, 16},
        {65475, 16},
        {65476, 16},
        {65477, 16},
        {65478, 16},
        {65479, 16},
        {65480, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {504,   9},
        {65481, 16},
        {65482, 16},
        {65483, 16},
        {65484, 16},
        {65485, 16},
        {65486, 16},
        {65487, 16},
        {65488, 16},
        {65489, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {505,   9},
        {65490, 16},
        {65491, 16},
        {65492, 16},
        {65493, 16},
        {65494, 16},
        {65495, 16},
        {65496, 16},
        {65497, 16},
        {65498, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {506,   9},
        {65499, 16},
        {65500, 16},
        {65501, 16},
        {65502, 16},
        {65503, 16},
        {65504, 16},
        {65505, 16},
        {65506, 16},
        {65507, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {2041,  11},
        {65508, 16},
        {65509, 16},
        {65510, 16},
        {65511, 16},
        {65512, 16},
        {65513, 16},
        {65514, 16},
        {65515, 16},
        {65516, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {16352, 14},
        {65517, 16},
        {65518, 16},
        {65519, 16},
        {65520, 16},
        {65521, 16},
        {65522, 16},
        {65523, 16},
        {65524, 16},
        {65525, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {1018,  10},
        {32707, 15},
        {65526, 16},
        {65527, 16},
        {65528, 16},
        {65529, 16},
        {65530, 16},
        {65531, 16},
        {65532, 16},
        {65533, 16},
        {65534, 16},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0},
        {0,     0}
};


// quantization table
static const uint32_t quantization_table_y[] = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99
};
static const int quantization_table_uv[] = {
        17, 18, 24, 47, 99, 99, 99, 99,
        18, 21, 26, 66, 99, 99, 99, 99,
        24, 26, 56, 99, 99, 99, 99, 99,
        47, 66, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99
};

static const float aasf[] = {
        1.0f * 2.828427125f,
        1.387039845f * 2.828427125f,
        1.306562965f * 2.828427125f,
        1.175875602f * 2.828427125f,
        1.0f * 2.828427125f,
        0.785694958f * 2.828427125f,
        0.541196100f * 2.828427125f,
        0.275899379f * 2.828427125f
};


static void jpeg_write_bits(uint32_t *bit_buffer, uint32_t *num_of_bits,
                            const uint16_t *bs, FILE *fp) {
    (*num_of_bits) += bs[1];
    (*bit_buffer) |= bs[0] << (24 - (*num_of_bits));
    while ((*num_of_bits) >= 8) {
        uint8_t c = ((*bit_buffer) >> 16) & 255;
        fputc(c, fp);
        if (c == 255) {
            // 0x00 must be included after 0xFF
            fputc(0x00, fp);
        }
        (*bit_buffer) <<= 8;
        (*num_of_bits) -= 8;
    }
}

static void jpeg_calc_bits(int32_t val, uint16_t bits[2]) {
    int32_t tmp = (val < 0) ? (-val) : val;
    val = (val < 0) ? (val - 1) : val;
    bits[1] = 1;
    while (tmp >>= 1) {
        ++bits[1];
    }
    bits[0] = val & ((1 << bits[1]) - 1);
}

static void jpeg_dct1(float *d0, float *d1, float *d2, float *d3,
                      float *d4, float *d5, float *d6, float *d7) {
    float tmp0 = (*d0) + (*d7);
    float tmp7 = (*d0) - (*d7);
    float tmp1 = (*d1) + (*d6);
    float tmp6 = (*d1) - (*d6);
    float tmp2 = (*d2) + (*d5);
    float tmp5 = (*d2) - (*d5);
    float tmp3 = (*d3) + (*d4);
    float tmp4 = (*d3) - (*d4);

    // even part
    float tmp10 = tmp0 + tmp3;
    float tmp13 = tmp0 - tmp3;
    float tmp11 = tmp1 + tmp2;
    float tmp12 = tmp1 - tmp2;

    (*d0) = tmp10 + tmp11;
    (*d4) = tmp10 - tmp11;

    float z1 = (tmp12 + tmp13) * 0.707106781f;
    (*d2) = tmp13 + z1;
    (*d6) = tmp13 - z1;

    // odd part
    tmp10 = tmp4 + tmp5;
    tmp11 = tmp5 + tmp6;
    tmp12 = tmp6 + tmp7;

    float z5 = (tmp10 - tmp12) * 0.382683433f;
    float z2 = tmp10 * 0.541196100f + z5;
    float z4 = tmp12 * 1.306562965f + z5;
    float z3 = tmp11 * 0.707106781f;

    float z11 = tmp7 + z3;
    float z13 = tmp7 - z3;

    (*d5) = z13 + z2;
    (*d3) = z13 - z2;
    (*d1) = z11 + z4;
    (*d7) = z11 - z4;
}

static int32_t jpeg_process_dct_unit(uint32_t *bit_buffer,
                                     uint32_t *num_of_bits, float *dct_unit, float *fast_dct_table,
                                     int32_t dc_value, const uint16_t huffman_dc[256][2],
                                     const uint16_t huffman_ac[256][2], FILE *fp, uint8_t step, uint8_t wipe) {
    uint32_t i;

    const uint16_t eob[2] = {huffman_ac[0x00][0], huffman_ac[0x00][1]};
    const uint16_t m16_zeroes[2] = {huffman_ac[0xF0][0], huffman_ac[0xF0][1]};

    // dct rows
    uint32_t data_offset;
    for (data_offset = 0; data_offset < 64; data_offset += 8) {
        jpeg_dct1(dct_unit + data_offset, dct_unit + data_offset + 1,
                  dct_unit + data_offset + 2, dct_unit + data_offset + 3,
                  dct_unit + data_offset + 4, dct_unit + data_offset + 5,
                  dct_unit + data_offset + 6, dct_unit + data_offset + 7);
    }

    // dct colums
    for (data_offset = 0; data_offset < 8; data_offset++) {
        jpeg_dct1(dct_unit + data_offset, dct_unit + data_offset + 8,
                  dct_unit + data_offset + 16, dct_unit + data_offset + 24,
                  dct_unit + data_offset + 32, dct_unit + data_offset + 40,
                  dct_unit + data_offset + 48, dct_unit + data_offset + 56);
    }

// -----------------------------------------------------------------------------
// case 2 experiments here
// - experiment 1: along u axis
// - experiment 2: along v axis
// - experiment 3: along u,v axis				
// -----------------------------------------------------------------------------
#ifdef EXPERIMENT_CASE_2
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
#endif

    // quantize/descale/zigzag the coefficients
    int32_t unit_out[64];
    for (i = 0; i < 64; i++) {
        float val = dct_unit[i] * fast_dct_table[i];
        unit_out[zigzag_index[i]] =
                (int32_t) ((val < 0) ? ceilf(val - 0.5f) : floorf(val + 0.5f));
    }

// -----------------------------------------------------------------------------
// case 1 experiments here (part B)
// - if wipe variable is set then 64 coefficients will be zero	
// -----------------------------------------------------------------------------
    if (wipe) {
        memset(unit_out, 0x00, sizeof(unit_out));
    }

    // encode dc value
    int32_t diff = unit_out[0] - dc_value;
    if (diff == 0) {
        jpeg_write_bits(bit_buffer, num_of_bits, huffman_dc[0], fp);
    } else {
        uint16_t bits[2];
        jpeg_calc_bits(diff, bits);
        jpeg_write_bits(bit_buffer, num_of_bits, huffman_dc[bits[1]], fp);
        jpeg_write_bits(bit_buffer, num_of_bits, bits, fp);
    }

    // encode ac
    uint32_t end_0_pos = 63;
    while ((end_0_pos > 0) && (unit_out[end_0_pos] == 0)) {
        end_0_pos--;
    }

    // end_0_pos = first element in reverse order != 0
    if (end_0_pos == 0) {
        jpeg_write_bits(bit_buffer, num_of_bits, eob, fp);
        return unit_out[0];
    }

    for (i = 1; i <= end_0_pos; i++) {
        uint32_t start_pos = i;
        while ((unit_out[i] == 0) && (i <= end_0_pos)) {
            i++;
        }

        uint32_t nr_zeroes = i - start_pos;
        if (nr_zeroes >= 16) {
            uint32_t lng = nr_zeroes >> 4, nr_marker;
            for (nr_marker = 1; nr_marker <= lng; nr_marker++) {
                jpeg_write_bits(bit_buffer, num_of_bits, m16_zeroes, fp);
            }
            nr_zeroes &= 15;
        }

        uint16_t bits[2];
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

int jpeg_exp_write(bmp_context_t *context, const char *dest, int step) {
    uint32_t i, j, k, row, col, x, y;

    if (context == NULL) {
        return -1;
    }

    FILE *fp = fopen(dest, "wb");
    if (fp == NULL) {
        return -1;
    }

    // actual quality value
    int32_t quality = (JPEG_QUALITY_PERCENT < 50) ?
                      (5000 / JPEG_QUALITY_PERCENT) : (200 - JPEG_QUALITY_PERCENT * 2);

    // Luminance table of quality applied quantization table
    uint8_t y_table[64];
    uint8_t uv_table[64];
    for (i = 0; i < 64; i++) {
        uint32_t yti = (quantization_table_y[i] * quality + 50) / 100;
        y_table[zigzag_index[i]] = (yti < 1) ? 1 : ((yti > 255) ? 255 : yti);

        uint32_t uvti = (quantization_table_uv[i] * quality + 50) / 100;
        uv_table[zigzag_index[i]] = (uvti < 1) ? 1 : ((uvti > 255) ? 255 : uvti);
    }

    // fast dct table
    float fast_dct_table_y[64];
    float fast_dct_table_uv[64];
    for (row = 0, k = 0; row < 8; row++) {
        for (col = 0; col < 8; col++, k++) {
            fast_dct_table_y[k] =
                    1 / (y_table[zigzag_index[k]] * aasf[row] * aasf[col]);
            fast_dct_table_uv[k] =
                    1 / (uv_table[zigzag_index[k]] * aasf[row] * aasf[col]);
        }
    }

    // write headers
    uint32_t width = context->header.bmp_width;
    uint32_t height = context->header.bmp_height;

    // SOI, APP0 JFIF
    static const uint8_t soi_app0_header[] = {
            0xFF, 0xD8, 0xFF, 0xE0, 0, 0x10, 'J', 'F', 'I', 'F', 0, 1, 1, 0, 0, 1, 0, 1, 0, 0
    };
    fwrite(soi_app0_header, sizeof(soi_app0_header), 1, fp);

    // DQT <marker> <length>
    static const uint8_t dqt_header[] = {0xFF, 0xDB, 0x00, 0x84};
    fwrite(dqt_header, sizeof(dqt_header), 1, fp);
    // DQT table Y
    fputc(0, fp); // <ID>
    fwrite(y_table, sizeof(y_table), 1, fp); // Table
    // DQT table UV
    fputc(1, fp);
    fwrite(uv_table, sizeof(uv_table), 1, fp);

    // SOF
    uint16_t be16;
    static const uint8_t sof_header[] = {0xFF, 0xC0, 0x00, 0x11};
    fwrite(sof_header, sizeof(sof_header), 1, fp);
    fputc(0x08, fp); // sampling bit
    // SOF-height
    be16 = htobe16(height);
    fwrite(&be16, 2, 1, fp);
    // SOF-width
    be16 = htobe16(width);
    fwrite(&be16, 2, 1, fp);
    // SOF-components
    fputc(0x03, fp); // YUV
    // SOF-component info <ID-comp,s_freq,ID-qtable>
    static const uint8_t cinfo[] = {
            0x01, 0x11, 0x00, // Y
            0x02, 0x11, 0x01, // U
            0x03, 0x11, 0x01  // V
    };
    fwrite(cinfo, sizeof(cinfo), 1, fp);

    // DHT
    static const uint8_t dht_header[] = {0xFF, 0xC4, 0x01, 0xA2};
    fwrite(dht_header, sizeof(dht_header), 1, fp);
    // DHT-huffman-codelength-counter-symbols DC Y
    fputc(0x00, fp); // <DC:Table-ID-0>
    fwrite(std_dc_luminance_nrcodes + 1,
           sizeof(std_dc_luminance_nrcodes) - 1, 1, fp);
    fwrite(std_dc_luminance_values, sizeof(std_dc_luminance_values), 1, fp);
    // DHT-huffman-codelength-counter-symbols AC Y
    fputc(0x10, fp); // <AC:Table-ID-0>
    fwrite(std_ac_luminance_nrcodes + 1,
           sizeof(std_ac_luminance_nrcodes) - 1, 1, fp);
    fwrite(std_ac_luminance_values, sizeof(std_ac_luminance_values), 1, fp);
    // DHT-huffman-codelength-counter-symbols DC UV
    fputc(0x01, fp);
    fwrite(std_dc_chrominance_nrcodes + 1,
           sizeof(std_dc_chrominance_nrcodes) - 1, 1, fp);
    fwrite(std_dc_chrominance_values,
           sizeof(std_dc_chrominance_values), 1, fp);
    // DHT-huffman-codelength-counter-symbols AC UV
    fputc(0x11, fp);
    fwrite(std_ac_chrominance_nrcodes + 1,
           sizeof(std_ac_chrominance_nrcodes) - 1, 1, fp);
    fwrite(std_ac_chrominance_values,
           sizeof(std_ac_chrominance_values), 1, fp);

    // SOS
    static const uint8_t sos_header[] = {0xFF, 0xDA, 0x00, 0x0C};
    fwrite(sos_header, sizeof(sos_header), 1, fp);
    // SOS-component-description
    static const uint8_t component_desc[] = {
            0x03, // component count
            0x01, 0x00, // Y component id, <DC-huffman-ID:AC-huffman-ID>
            0x02, 0x11, // U component id, <DC-huffman-ID:AC-huffman-ID>
            0x03, 0x11  // V component id, <DC-huffman-ID:AC-huffman-ID>
    };
    fwrite(component_desc, sizeof(component_desc), 1, fp);
    // SOS-DCT-optimize-value
    static const uint8_t sos_dct_val[] = {0x00, 0x3F, 0x00};
    fwrite(sos_dct_val, sizeof(sos_dct_val), 1, fp);

    // encode 8x8 macroblocks
    uint32_t bit_buffer = 0, num_of_bits = 0, pos = 0;
    int32_t dc_y = 0, dc_u = 0, dc_v = 0; // dc component of Y

    // for blocks
    for (y = 0; y < height; y += 8) {
        for (x = 0; x < width; x += 8) {
            // 8x8 dct unit
            float dct_unit_y[64];
            float dct_unit_u[64];
            float dct_unit_v[64];

            // compute dct unit
            for (row = y, pos = 0; row < y + 8; row++) {
                for (col = x; col < x + 8; col++, pos++) {
                    int32_t p = row * width + col;
                    if (row >= height) {
                        p -= width * (row + 1 - height);
                    }
                    if (col >= width) {
                        p -= (col + 1 - width);
                    }

                    float r = context->channel_red[p];
                    float g = context->channel_green[p];
                    float b = context->channel_blue[p];

                    // rgb to yuv
                    dct_unit_y[pos] = +0.29900f * r + 0.58700f * g + 0.11400f * b - 128;
                    dct_unit_u[pos] = -0.14700f * r - 0.28900f * g + 0.43600f * b;
                    dct_unit_v[pos] = +0.61500f * r - 0.51500f * g - 0.10000f * b;
                }
            }
// -----------------------------------------------------------------------------
// case 1 experiments here (code part A)
// - experiment 1: along u axis
// - experiment 2: along v axis
// - experiment 3: along u,v axis			
// - if wipe variable is set then 64 coefficients will be zero
// -----------------------------------------------------------------------------
            uint8_t wipe = 0;
#ifdef EXPERIMENT_CASE_1
#ifdef EXPERIMENT_NUM_1
            uint32_t step_idx = (step + 1) * (64);
            if (x >= step_idx) {
                // turn into zero
                wipe = 1;
            }
#endif
#ifdef EXPERIMENT_NUM_2
            uint32_t step_idx = (step + 1) * (64);
            if (y >= step_idx) {
                // turn into zero
                wipe = 1;
            }
#endif
#ifdef EXPERIMENT_NUM_3
            uint32_t step_idx = (step + 1) * (64);
            if (y >= step_idx || x >= step_idx) {
                // turn into zero
                wipe = 1;
            }
#endif
#endif
            // encode and write
            dc_y = jpeg_process_dct_unit(&bit_buffer, &num_of_bits, dct_unit_y,
                                         fast_dct_table_y, dc_y, huffman_dc_y, huffman_ac_y, fp, step, wipe);
            dc_u = jpeg_process_dct_unit(&bit_buffer, &num_of_bits, dct_unit_u,
                                         fast_dct_table_uv, dc_u, huffman_dc_uv, huffman_ac_uv, fp, step, wipe);
            dc_v = jpeg_process_dct_unit(&bit_buffer, &num_of_bits, dct_unit_v,
                                         fast_dct_table_uv, dc_v, huffman_dc_uv, huffman_ac_uv, fp, step, wipe);
        }
    }

    // bit alignment of the eoi marker
    static const uint16_t filler[] = {0x7F, 0x07};
    jpeg_write_bits(&bit_buffer, &num_of_bits, filler, fp);

    // EOI
    fputc(0xFF, fp);
    fputc(0xD9, fp);


    fclose(fp);
    return 0;
}