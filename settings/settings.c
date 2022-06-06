//
// Created by SNAPDRAGON_11800H on 3/15/2022.
//

#include "settings.h"

unsigned char YCbCrY[MAX][MAX];
unsigned char Cb[MAX][MAX];
unsigned char Cr[MAX][MAX];

unsigned char YuvY[MAX][MAX];
unsigned char U[MAX][MAX];
unsigned char V[MAX][MAX];

unsigned char r[MAX][MAX], output_r[MAX][MAX];
unsigned char g[MAX][MAX], output_g[MAX][MAX];
unsigned char b[MAX][MAX], output_b[MAX][MAX];
unsigned char records[MAX][MAX];

unsigned char gray[MAX][MAX];

char *SourceFile_Path, *OutputFile_Path;
const char *OutputFile_Down = "-output.bmp";
const char *BmpBoard = "../fileIO/bmpBoard.profile";