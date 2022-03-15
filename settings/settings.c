//
// Created by SNAPDRAGON_11800H on 3/15/2022.
//

#include <stdio.h>

#include "settings.h"

unsigned int OffSet;
long width;
long height;

unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];
unsigned char r[MAXSIZE][MAXSIZE], output_r[MAXSIZE][MAXSIZE];
unsigned char g[MAXSIZE][MAXSIZE], output_g[MAXSIZE][MAXSIZE];
unsigned char b[MAXSIZE][MAXSIZE], output_b[MAXSIZE][MAXSIZE];
unsigned char records[MAXSIZE][MAXSIZE];