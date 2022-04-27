//
// Created by snapdragon_manjaro21 on 4/20/2022.
//

#include "tf.h"

float **dat_fl;
float *dat_fl_l;

float **dat_fl, **matimg, **modulus;
float *mat_line, *mod_line;

float **matdct;
float *dct_line;

void fftinit(int height, int width) {
    matimg = (float **) malloc(sizeof(float *) * height);
    mat_line = (float *) malloc(sizeof(float) * width * height);

    modulus = (float **) malloc(sizeof(float *) * height);
    mod_line = (float *) malloc(sizeof(float) * width * height);

    dat_fl = (float **) malloc(sizeof(float *) * height);
    dat_fl_l = (float *) malloc(sizeof(float) * width * height);

    for (int i = 0; i < height; i++, mat_line += width, mod_line += width, dat_fl_l += width) {
        matimg[i] = mat_line;
        modulus[i] = mod_line;
        dat_fl[i] = dat_fl_l;
    }
}

void dctinit(int height, int width) {
    matdct = (float **) malloc(sizeof(float *) * height);
    dct_line = (float *) malloc(sizeof(float) * width * height);

    dat_fl = (float **) malloc(sizeof(float *) * height);
    dat_fl_l = (float *) malloc(sizeof(float) * width * height);

    for (int i = 0; i < height; i++, dct_line += width) {
        matdct[i] = dct_line;
        dat_fl[i] = dat_fl_l;
    }
}

void fourrier(float data[], int lw[], int sign) {
    int idim;
    unsigned long i1, i2, i3, i2rev, i3rev, ip1, ip2, ip3, ifp1, ifp2;
    unsigned long ibit, k1, k2, n, nprev, nrem, ntot;
    float tempi, tempr;
    double theta, wi, wpi, wpr, wr, wtemp;

    ntot = lw[0] * lw[1];

    nprev = 1;

    for (idim = 1; idim >= 0; idim--) {
        n = lw[idim];
        nrem = ntot / (n * nprev);
        ip1 = nprev << 1;
        ip2 = ip1 * n;
        ip3 = ip2 * nrem;
        i2rev = 1;

        for (i2 = 1; i2 <= ip2; i2 += ip1) {
            if (i2 < i2rev) {
                for (i1 = i2; i1 <= i2 + ip1 - 2; i1 += 2) {
                    for (i3 = i1; i3 <= ip3; i3 += ip2) {
                        i3rev = i2rev + i3 - i2;
                        swap(data[i3], data[i3rev]);
                        swap(data[i3 + 1], data[i3rev + 1]);
                    }
                }
            }

            ibit = ip2 >> 1;

            while (ibit >= ip1 && i2rev > ibit) {
                i2rev -= ibit;
                ibit >>= 1;
            }

            i2rev += ibit;
        }

        ifp1 = ip1;

        while (ifp1 < ip2) {
            ifp2 = ifp1 << 1;
            theta = sign * 6.28318530717959 / (ifp2 / ip1);
            wtemp = sin(0.5 * theta);
            wpr = -2.0 * wtemp * wtemp;
            wpi = sin(theta);
            wr = 1.0;
            wi = 0.0;

            for (i3 = 1; i3 <= ifp1; i3 += ip1) {
                for (i1 = i3; i1 <= i3 + ip1 - 2; i1 += 2) {
                    for (i2 = i1; i2 <= ip3; i2 += ifp2) {
                        k1 = i2;
                        k2 = k1 + ifp1;
                        tempr = (float) wr * data[k2] - (float) wi * data[k2 + 1];
                        tempi = (float) wr * data[k2 + 1] + (float) wi * data[k2];
                        data[k2] = data[k1] - tempr;
                        data[k2 + 1] = data[k1 + 1] - tempi;
                        data[k1] += tempr;
                        data[k1 + 1] += tempi;
                    }
                }

                wr = (wtemp = wr) * wpr - wi * wpi + wr;
                wi = wi * wpr + wtemp * wpi + wi;
            }
            ifp1 = ifp2;
        }
        nprev *= n;
    }
}

void fft(float **matreal, int height, int width) {
    int i, j, posx, posy;
    float *data;
    int *lw;

    data = (float *) malloc(sizeof(float) * (2 * width * height) + 1);
    lw = (int *) malloc(sizeof(int) * (2));

    lw[0] = height;
    lw[1] = width;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            data[2 * (i * height + j) + 1] = matreal[i][j];
            data[2 * (i * height + j) + 2] = matimg[i][j];
        }
    }

    fourrier(data, lw, FFT);

    for (i = 0; i < width * height; i++) {
        posy = i / width;
        posx = i % width;
        matreal[posy][posx] = data[(2 * i) + 1];
        matimg[posy][posx] = data[(2 * i) + 2];
    }

    free(data);
    free(lw);
}


void ifft(float **matreal, int height, int width) {
    int i, j, posx, posy;
    float *data;
    int *lw;

    data = (float *) malloc(sizeof(float) * (2 * width * height) + 1);
    lw = (int *) malloc(sizeof(int) * (2));

    lw[0] = height;
    lw[1] = width;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            data[2 * (i * height + j) + 1] = matreal[i][j];
            data[2 * (i * height + j) + 2] = matimg[i][j];
        }
    }

    fourrier(data, lw, IFFT);

    for (i = 0; i < width * height; i++) {
        posy = i / width;
        posx = i % width;
        matreal[posy][posx] = data[(2 * i) + 1] / (width * height);
        matimg[posy][posx] = data[(2 * i) + 2] / (width * height);
    }

    free(data);
    free(lw);
}

void mod(float **modout, float **matreal, int height, int width) {
    int i, j;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            modout[i][j] = sqrt(matreal[i][j] * matreal[i][j] + matimg[i][j] * matimg[i][j]);
        }
    }
}

void fftshift(float **mat, int height, int width) {
    int i, j;
    float max, min;

    min = mat[0][0];

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (mat[i][j] < min) {
                min = mat[i][j];
            }
        }
    }

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            mat[i][j] -= min;
        }
    }

    max = mat[0][0];

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (mat[i][j] > max) {
                max = mat[i][j];
            }
        }
    }

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            mat[i][j] *= (255 / max);
        }
    }
}

void mult(float **mat, int coef, int height, int width) {
    int i, j;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            mat[i][j] *= coef;

            if (mat[i][j] > 255) {
                mat[i][j] = 255;
            }
        }
    }
}

void center(float **in, float **out, int length, int width) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (i < length / 2) {
                if (j < width / 2) {
                    out[i][j] = in[i + length / 2][j + width / 2];
                } else {
                    out[i][j] = in[i + length / 2][j - width / 2];
                }
            } else {
                if (j < width / 2) {
                    out[i][j] = in[i - length / 2][j + width / 2];
                } else {
                    out[i][j] = in[i - length / 2][j - width / 2];
                }
            }
        }
    }
}

void dct(float **in, int height, int width) {
    for (int u = 0; u < height; u++) {
        for (int v = 0; v < width; v++) {
            matdct[u][v] = 0;
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    matdct[u][v] += in[i][j] * cos(M_PI / ((float) height) * (i + 1.0 / 2.0) * u) *
                                    cos(M_PI / ((float) width) * (j + 1.0 / 2.0) * v);
                }
            }
        }
    }
}

void idct(float **out, int height, int width) {
    int i, j, u, v;

    for (u = 0; u < height; ++u) {
        for (v = 0; v < width; ++v) {
            out[u][v] = 1 / 4.0 * matdct[0][0];
            for (i = 1; i < height; i++) {
                out[u][v] += 1 / 2.0 * matdct[i][0];
            }
            for (j = 1; j < width; j++) {
                out[u][v] += 1 / 2.0 * matdct[0][j];
            }

            for (i = 1; i < height; i++) {
                for (j = 1; j < width; j++) {
                    out[u][v] += matdct[i][j] * cos(M_PI / ((float) height) * (u + 1.0 / 2.0) * i) *
                                 cos(M_PI / ((float) height) * (v + 1.0 / 2.0) * j);
                }
            }
            out[u][v] *= 2 / ((float) width) * 2.0 / ((float) height);
        }
    }
}