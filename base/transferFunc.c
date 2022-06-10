//
// Created by SNAPDRAGON_11800H on 3/18/2022.
//

#include "transferFunc.h"

void RGB2Gray(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char gray[][MAX]) {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            gray[j][i] = 0.299 * r[j][i] + 0.587 * g[j][i] + 0.114 * b[j][i];
        }
    }
}

void RGB2YUV(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char YuvY[][MAX],
             unsigned char U[][MAX], unsigned char V[][MAX]) {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = 0.299 * r[j][i] + 0.587 * g[j][i] + 0.114 * b[j][i];
            U[j][i] = -0.147 * r[j][i] - 0.289 * g[j][i] + 0.436 * b[j][i];
            V[j][i] = 0.615 * r[j][i] - 0.515 * g[j][i] - 0.100 * b[j][i];
        }
    }
}

void YUV2Gray(unsigned char YuvY[][MAX], unsigned char U[][MAX], unsigned char V[][MAX], unsigned char r[][MAX],
              unsigned char g[][MAX], unsigned char b[][MAX]) {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            /* YuvY[j][i] = YuvY[j][i]; */
            U[j][i] = 0;
            V[j][i] = 0;
        }
    }
    YUV2RGB(YuvY, U, V, r, g, b);
}

void RGB2YCbCr(unsigned char r[][MAX], unsigned char g[][MAX], unsigned char b[][MAX], unsigned char YCbCrY[][MAX],
               unsigned char Cb[][MAX], unsigned char Cr[][MAX]) {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YCbCrY[j][i] = 0.299 * r[j][i] + 0.587 * g[j][i] + 0.114 * b[j][i] - 128;
            Cb[j][i] = -0.1687 * r[j][i] - 0.3313 * g[j][i] + 0.500 * b[j][i];
            Cr[j][i] = 0.500 * r[j][i] - 0.4187 * g[j][i] - 0.0813 * b[j][i];
        }
    }
}

void YUV2RGB(unsigned char YuvY[][MAX], unsigned char U[][MAX], unsigned char V[][MAX], unsigned char r[][MAX],
             unsigned char g[][MAX], unsigned char b[][MAX]) {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            r[j][i] = YuvY[j][i] + 1.140 * V[j][i];
            g[j][i] = YuvY[j][i] - 0.394 * U[j][i] - 0.581 * V[j][i];
            b[j][i] = YuvY[j][i] + 2.032 * U[j][i];
        }
    }
}

void YUV2Binarization(unsigned char YuvY[][MAX], unsigned char U[][MAX], unsigned char V[][MAX], unsigned char r[][MAX],
                      unsigned char g[][MAX], unsigned char b[][MAX]) {
    //OTSU algorithm
    unsigned char T = 0, perfect_T = 0; //Threshold
    double w0, w1, u0, u1, u;//paramenter
    long number_below_T = 0L, number_above_T = 0L; // Parameter
    double gt[255] = {0.0}, max_g = 0.0;//result;

    for (int num = 0; num <= 255; num++, T++) {
        //after this there is a max g
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
        /* Step 2 */
        w0 = (double) number_below_T / (bmpIHeader.bIHeight * bmpIHeader.bIWidth);
        w1 = (double) number_above_T / (bmpIHeader.bIHeight * bmpIHeader.bIWidth);
        u = w0 * u0 + w1 * u1;
        gt[num] = w0 * pow(abs(u0 - u), 2.0) + w1 * pow(abs(u1 - u), 2.0);
        /* Step 3 find max G */
        if (gt[num] > max_g) {
            max_g = gt[num];
            perfect_T = T;
        }
    }
    // '用算出来的 perfect_T 解出二值图'
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
    YUV2RGB(YuvY, U, V, r, g, b);
}