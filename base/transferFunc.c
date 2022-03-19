//
// Created by SNAPDRAGON_11800H on 3/18/2022.
//

#include "transferFunc.h"

void RGB2Gray() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            gray[j][i] = 0.299 * r[j][i] + 0.587 * g[j][i] + 0.114 * b[j][i];
        }
    }
}

void RGB2YUV() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = 0.299 * r[j][i] + 0.587 * g[j][i] + 0.114 * b[j][i];
            U[j][i] = -0.147 * r[j][i] - 0.289 * g[j][i] + 0.436 * b[j][i];
            V[j][i] = 0.615 * r[j][i] - 0.515 * g[j][i] - 0.100 * b[j][i];
        }
    }
}

void YUV2Gray() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            /* YuvY[j][i] = YuvY[j][i]; */
            U[j][i] = 0;
            V[j][i] = 0;
        }
    }
}

void YUV2RGB() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            r[j][i] = YuvY[j][i] + 1.140 * V[j][i];
            g[j][i] = YuvY[j][i] - 0.394 * U[j][i] - 0.581 * V[j][i];
            b[j][i] = YuvY[j][i] + 2.032 * U[j][i];
        }
    }
}

void YUV2Binarization()//output YUV
{

    //OTSU althrithm '大津算法'
    unsigned char T = 0, perfect_T = 0;//Threshold
    double w0, w1, u0, u1, u;//paramenter
    long number_below_T = 0L, number_above_T = 0L;//paramenter
    double g[255] = {0.0}, max_g = 0.0;//result;

    for (int num = 0; num <= 255; num++, T++)//after this there is a max g
    {
        /*Pre calcu*/
        long all_below_T = 0L, all_above_T = 0L;
        number_below_T = 0L, number_above_T = 0L;
        for (int j = 0; j < bmpIHeader.bIHeight; j++) {
            for (int i = 0; i < bmpIHeader.bIWidth; i++) {
                if (T >= YuvY[j][i]) {
                    number_below_T++;
                    all_below_T += YuvY[j][i];
                } else {
                    number_above_T++;
                    all_above_T += YuvY[j][i];
                }
            }
        }
        if (number_below_T == 0 || number_above_T == 0) continue;
        u0 = (double) all_below_T / number_below_T;
        u1 = (double) all_above_T / number_above_T;
        /*Step2*/
        w0 = (double) number_below_T / (bmpIHeader.bIHeight * bmpIHeader.bIWidth);
        w1 = (double) number_above_T / (bmpIHeader.bIHeight * bmpIHeader.bIWidth);
        u = w0 * u0 + w1 * u1;
        g[num] = w0 * pow(abs(u0 - u), 2.0) + w1 * pow(abs(u1 - u), 2.0);
        /*Step 3 find max g*/
        if (g[num] > max_g) {
            max_g = g[num];
            perfect_T = T;
        }
    }
    //'用算出来的perfect_T解出二值图'
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            if (YuvY[j][i] < perfect_T)
                YuvY[j][i] = 0;
            else
                YuvY[j][i] = 255;
            U[j][i] = 0;
            V[j][i] = 0;
        }
    }
    printf("Binarization finished!\n");
}