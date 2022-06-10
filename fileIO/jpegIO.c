//
// Created by SNAPDRAGON_11800H on 6/6/2022.
//

#include "jpegIO.h"

unsigned char y_table[64], cbcr_table[64];
float fast_dct_table_y[64], fast_dct_table_cbcr[64];

// quantization table
const unsigned int quantization_table_y[] = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99
};
const unsigned int quantization_table_uv[] = {
        17, 18, 24, 47, 99, 99, 99, 99,
        18, 21, 26, 66, 99, 99, 99, 99,
        24, 26, 56, 99, 99, 99, 99, 99,
        47, 66, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99
};

// to index zigzag scanning conveniently
const unsigned char zigzag_index[] = {
        0, 1, 5, 6, 14, 15, 27, 28, 2, 4, 7, 13, 16, 26,
        29, 42, 3, 8, 12, 17, 25, 30, 41, 43, 9, 11, 18, 24,
        31, 40, 44, 53, 10, 19, 23, 32, 39, 45, 52, 54, 20,
        22, 33, 38, 46, 51, 55, 60, 21, 34, 37, 47, 50, 56,
        59, 61, 35, 36, 48, 49, 57, 58, 62, 63
};

// encoding DC_Y
const unsigned char std_dc_luminance_nrcodes[] = {
        0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
};
const unsigned char std_dc_luminance_values[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

// encoding AC_Y
const unsigned char std_ac_luminance_nrcodes[] = {
        0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d
};
const unsigned char std_ac_luminance_values[] = {
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

// encoding DC_UV(CbCr)
const unsigned char std_dc_chrominance_nrcodes[] = {
        0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
};
const unsigned char std_dc_chrominance_values[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

// encoding AC_UV(CbCr)
const unsigned char std_ac_chrominance_nrcodes[] = {
        0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77
};
const unsigned char std_ac_chrominance_values[] = {
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

// Huffman tables DC_Y
const unsigned short huffman_dc_y[256][2] = {
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
// Huffman tables AC_Y
const unsigned short huffman_ac_y[256][2] = {
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

// Huffman tables DC_UV(CbCr)
const unsigned short huffman_dc_uv[256][2] = {
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
// Huffman tables AC_UV(CbCr)
const unsigned short huffman_ac_uv[256][2] = {
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

// Optimize for division
const float aasf[] = {
        1.0f * 2.828427125f,
        1.387039845f * 2.828427125f,
        1.306562965f * 2.828427125f,
        1.175875602f * 2.828427125f,
        1.0f * 2.828427125f,
        0.785694958f * 2.828427125f,
        0.541196100f * 2.828427125f,
        0.275899379f * 2.828427125f
};

const unsigned char soi_app0_header[] = {
        0xFF, 0xD8, 0xFF, 0xE0, 0x0, 0x10, 0x4A, 0x46, 0x49, 0x46,
        0, 1, 1, 0, 0, 1, 0, 1, 0, 0
};
const unsigned char dqt_header[] = {0xFF, 0xDB, 0x00, 0x84};
const unsigned char sof_header[] = {0xFF, 0xC0, 0x00, 0x11};
const unsigned char cinfo[] = {
        0x01, 0x11, 0x00, // Y
        0x02, 0x11, 0x01, // Cb
        0x03, 0x11, 0x01  // Cr
};
const unsigned char dht_header[] = {0xFF, 0xC4, 0x01, 0xA2};
const unsigned char sos_header[] = {0xFF, 0xDA, 0x00, 0x0C};
const unsigned char component_desc[] = {
        0x03, // component count
        0x01, 0x00, // Y component id, <DC-huffman-ID:AC-huffman-ID>
        0x02, 0x11, // Cb component id, <DC-huffman-ID:AC-huffman-ID>
        0x03, 0x11  // Cr component id, <DC-huffman-ID:AC-huffman-ID>
};
const unsigned char sos_dct_val[] = {0x00, 0x3F, 0x00};
const unsigned char eoi_lst[] = {0xFF, 0xD9, };

void gen_tables(int quality) {
    // Luminance table of quality applied quantization table
    for (int i = 0; i < 64; i++) {
        unsigned int yti = (quantization_table_y[i] * quality + 50) / 100;
        y_table[zigzag_index[i]] = (yti < 1) ? 1 : ((yti > 255) ? 255 : yti);

        unsigned int cbcrti = (quantization_table_uv[i] * quality + 50) / 100;
        cbcr_table[zigzag_index[i]] = (cbcrti < 1) ? 1 : ((cbcrti > 255) ? 255 : cbcrti);
    }

    // fast dct table
    for (int row = 0, k = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++, k++) {
            fast_dct_table_y[k] = 1 / (y_table[zigzag_index[k]] * aasf[row] * aasf[col]);
            fast_dct_table_cbcr[k] = 1 / (cbcr_table[zigzag_index[k]] * aasf[row] * aasf[col]);
        }
    }
}

void addJpegHeader(FILE *jpegIOBuf) {
    // SOI, APP0
    fwrite(soi_app0_header, sizeof(soi_app0_header), 1, jpegIOBuf);


    // DQT <marker ID> <length>
    fwrite(dqt_header, sizeof(dqt_header), 1, jpegIOBuf);

    // DQT table Y
    fputc(0, jpegIOBuf); // <ID>
    fwrite(y_table, sizeof(y_table), 1, jpegIOBuf); // Table

    // DQT table CbCr
    fputc(1, jpegIOBuf);
    fwrite(cbcr_table, sizeof(cbcr_table), 1, jpegIOBuf);


    // SOF
    fwrite(sof_header, sizeof(sof_header), 1, jpegIOBuf);
    fputc(0x08, jpegIOBuf); // sampling bit

    // SOF-height
    unsigned short height_l = bmpIHeader.bIHeight >> 8;
    unsigned short height_r = (bmpIHeader.bIHeight << 24) >> 24;
    fwrite(&height_l, 1, 1, jpegIOBuf);
    fwrite(&height_r, 1, 1, jpegIOBuf);
    // SOF-width
    unsigned short width_l = bmpIHeader.bIWidth >> 8;
    unsigned short width_r = (bmpIHeader.bIWidth << 24) >> 24;
    fwrite(&width_l, 1, 1, jpegIOBuf);
    fwrite(&width_r, 1, 1, jpegIOBuf);

    // SOF-components
    fputc(0x03, jpegIOBuf); // YCbCr
    // SOF-component info <ID-comp,s_freq,ID-qtable>
    fwrite(cinfo, sizeof(cinfo), 1, jpegIOBuf);


    // DHT
    fwrite(dht_header, sizeof(dht_header), 1, jpegIOBuf);

    // DHT-huffman-codelength-counter-symbols DC Y
    fputc(0x00, jpegIOBuf); // <DC:Table-ID-0>
    fwrite(std_dc_luminance_nrcodes + 1,
           sizeof(std_dc_luminance_nrcodes) - 1, 1, jpegIOBuf);
    fwrite(std_dc_luminance_values, sizeof(std_dc_luminance_values), 1, jpegIOBuf);

    // DHT-huffman-codelength-counter-symbols AC Y
    fputc(0x10, jpegIOBuf); // <AC:Table-ID-0>
    fwrite(std_ac_luminance_nrcodes + 1,
           sizeof(std_ac_luminance_nrcodes) - 1, 1, jpegIOBuf);
    fwrite(std_ac_luminance_values, sizeof(std_ac_luminance_values), 1, jpegIOBuf);

    // DHT-huffman-codelength-counter-symbols DC CbCr
    fputc(0x01, jpegIOBuf);
    fwrite(std_dc_chrominance_nrcodes + 1,
           sizeof(std_dc_chrominance_nrcodes) - 1, 1, jpegIOBuf);
    fwrite(std_dc_chrominance_values,
           sizeof(std_dc_chrominance_values), 1, jpegIOBuf);

    // DHT-huffman-codelength-counter-symbols AC CbCr
    fputc(0x11, jpegIOBuf);
    fwrite(std_ac_chrominance_nrcodes + 1,
           sizeof(std_ac_chrominance_nrcodes) - 1, 1, jpegIOBuf);
    fwrite(std_ac_chrominance_values,
           sizeof(std_ac_chrominance_values), 1, jpegIOBuf);


    // SOS
    fwrite(sos_header, sizeof(sos_header), 1, jpegIOBuf);
    // SOS-component-description
    fwrite(component_desc, sizeof(component_desc), 1, jpegIOBuf);
    // SOS-DCT-optimize-value
    fwrite(sos_dct_val, sizeof(sos_dct_val), 1, jpegIOBuf);
}

void fillJpegLast(FILE *jpegIOBuf) {
    fwrite(eoi_lst, sizeof(eoi_lst), 1, jpegIOBuf);
}

void jpeg_write_bits(unsigned int *bit_buffer, unsigned int *num_of_bits, const unsigned short *bs, FILE *fp) {
    (*num_of_bits) += bs[1];
    (*bit_buffer) |= bs[0] << (24 - (*num_of_bits));
    while ((*num_of_bits) >= 8) {
        unsigned char c = ((*bit_buffer) >> 16) & 255;
        fputc(c, fp);
        if (c == 255) {
            // 0x00 must be included after 0xFF
            fputc(0x00, fp);
        }
        (*bit_buffer) <<= 8;
        (*num_of_bits) -= 8;
    }
}

void jpeg_calc_bits(int val, unsigned short bits[2]) {
    int tmp = (val < 0) ? (-val) : val;
    val = (val < 0) ? (val - 1) : val;
    bits[1] = 1;
    while (tmp >>= 1) {
        ++bits[1];
    }
    bits[0] = val & ((1 << bits[1]) - 1);
}