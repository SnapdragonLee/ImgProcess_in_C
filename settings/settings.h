//
// Created by SNAPDRAGON_11800H on 3/15/2022.
//

#ifndef IMGPROCESS_IN_C_SETTINGS_H
#define IMGPROCESS_IN_C_SETTINGS_H

#define MAXSIZE 10000
#define elementline 3

extern unsigned int OffSet;
extern long width;
extern long height;


extern unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
extern unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
extern unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];

extern unsigned char r[MAXSIZE][MAXSIZE], output_r[MAXSIZE][MAXSIZE];
extern unsigned char g[MAXSIZE][MAXSIZE], output_g[MAXSIZE][MAXSIZE];
extern unsigned char b[MAXSIZE][MAXSIZE], output_b[MAXSIZE][MAXSIZE];
extern unsigned char records[MAXSIZE][MAXSIZE];

#endif //IMGPROCESS_IN_C_SETTINGS_H
