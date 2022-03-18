//
// Created by snapdragon_manjaro21 on 2022/3/17.
//

#include "algoFunc.h"

#include "../settings/settings.h"

void HistogramEqualization(unsigned char **dat) {
    int cdf[256] = {0}, i, j;
    for (j = 0; j < bmpIHeader.bIHeight; j++) {
        for (i = 0; i < bmpIHeader.bIWidth; i++) {
            cdf[dat[j][i]]++;
        }
    }

    int cdfMin = cdf[0], cdfMax = &cdf[0], flagMin = 0;
    for (i = 1; i < 256; i++) {
        cdf[i] = cdf[i] + cdf[i - 1];
        if (cdf[i] > 1 && flagMin != 0) {

        }
    }

    dat;

}

void HistogramEqualization_r() {
    //number 统计
    long n0[256] = {0.0};
    /*0-255 直方图X3 RGB*/
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            n0[r[j][i]]++;
        }
    }

    /*0-1/255-...-254/255,1*/
    double p0[256] = {0.0};
    for (int i = 0; i < 256; i++)
        p0[i] = n0[i] / (1.0 * bmpIHeader.bIHeight * bmpIHeader.bIWidth);

    /*n[k]0-255:number*/

    /*Pn[n]*/
    double s0[256] = {0.0};
    s0[0] = p0[0];
    for (int i = 1; i < 256; i++)
        s0[i] = s0[i - 1] + p0[i];
//---------------------------------
    /*s[k]=求和Pn[0+..+k]*/
    /*找距离s[k]最近的像素点s[x]*/
    double minmize = 1.0;
    int min_number[256] = {0};
    double diff = 0.0;
    for (int j = 0; j < 256; j++) {
        //printf("!! == %f\n",s0[j]);
        diff = 0.0;
        minmize = 1.0;
        for (int i = 0; i < 256; i++) {
            double pix0 = 1.0 * i / 256;
            diff = fabs(s0[j] - pix0);
            //printf("%d == %f\n",i,diff);
            if (diff < minmize) {
                minmize = diff;
                min_number[j] = i;//min_number[j] --> i ;
            }

        }
        //printf("%d == %d\n",j,min_number[j]);
        //	break;
    }
    //输出
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            r[j][i] = min_number[r[j][i]];
        }
    }

    /*统计临近的相同的像素点*/
    /*计算最终的而结果并画直方图*/
}

void ChangeTheLuminance() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = 255 - YuvY[j][i];
            U[j][i] = U[j][i];
            V[j][i] = V[j][i];
        }
    }
}


void Dilation_Lining(char dowhat) {
    int Struture_Element[elementline][elementline] = {{0}};//假设是十字型
    int centre = (elementline - 1) / 2;
    /*自动生成十字形element*/
    for (int j = 0; j < elementline; j++) {
        for (int i = 0; i < elementline; i++) {
            if (j == centre || i == centre)
                Struture_Element[j][i] = 1;
            else
                Struture_Element[j][i] = 0;
        }
    }

    /*叠加原图*/
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = YuvY[j][i];
            U[j][i] = 0;
            V[j][i] = 0;
        }
    }

    /*膨胀一小圈*/
    for (int j = 0; j < bmpIHeader.bIHeight - elementline; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth - elementline; i++) {
            int Istrue = 0;
            for (int i1 = 0; i1 < elementline; i1++)
                for (int j1 = 0; j1 < elementline; j1++) {
                    if (Struture_Element[i1][j1] == 1 && YuvY[j + i1][i + j1] == 0)
                        Istrue = 1;
                }
            /*大概，YUV中黑色是。。。Y=0 （Ｔ＾Ｔ）*/
            if (Istrue == 1)
                YuvY[j + centre][i + centre] = dowhat == 'd' ? 0 : 255;

        }
    }
    printf("Dilation successed!\n");
}

void Erosion() {
    int Struture_Element[elementline][elementline] = {{0}};//假设是十字型
    int centre = (elementline - 1) / 2;
    /*自动生成十字形element*/
    for (int j = 0; j < elementline; j++) {
        for (int i = 0; i < elementline; i++) {
            if (j == centre || i == centre)
                Struture_Element[j][i] = 1;
            else
                Struture_Element[j][i] = 0;
        }
    }

    /*叠加原图*/
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = 255;
            U[j][i] = 0;
            V[j][i] = 0;
        }
    }

    /*缩小一小圈*/
    for (int j = 0; j < bmpIHeader.bIHeight - elementline; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth - elementline; i++) {
            int Istrue = 0;
            for (int i1 = 0; i1 < elementline; i1++)
                for (int j1 = 0; j1 < elementline; j1++) {
                    if (Struture_Element[i1][j1] == 0)
                        continue;
                    else if (Struture_Element[i1][j1] == 1 && YuvY[j + i1][i + j1] != 0)//不同时黑色
                        Istrue = 1;//不全等
                }
            //printf("%d",Istrue);
            /*大概，YUV中黑色是。。。Y=0 （Ｔ＾Ｔ）*/
            if (Istrue == 0)
                YuvY[j + centre][i + centre] = 0;//所有情况都成立！

        }
    }
    printf("Erosion successed!\n");
}

void Opening_Closing(char dowhat) {
    if (dowhat == 'o') {
        Erosion();
        Dilation_Lining('d');
        printf("Opening finished, Now outer noises are removed!\n");
    } else if (dowhat == 'c') {
        Dilation_Lining('d');
        Erosion();
        printf("Closing finished, Now inner noises are removed!\n");
    } else
        printf("Opening_Closing fail!\n");
}

//============================3==============================
void VisibilityEnhancement() {
    int maxLumnance = 0;
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            if (YuvY[j][i] > maxLumnance)
                maxLumnance = YuvY[j][i];
        }
    }

    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = 255 * (log10(YuvY[j][i] + 1)) / (log10(maxLumnance + 1));
            U[j][i] = 0;
            V[j][i] = 0;
        }
    }

}

void HistogramEqualization_g() {
    //number 统计
    long n0[256] = {0.0};
    /*0-255 直方图X3 RGB*/
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            n0[g[j][i]]++;
        }
    }

    /*0-1/255-...-254/255,1*/
    double p0[256] = {0.0};
    for (int i = 0; i < 256; i++)
        p0[i] = n0[i] / (1.0 * bmpIHeader.bIHeight * bmpIHeader.bIWidth);

    /*n[k]0-255:number*/

    /*Pn[n]*/
    double s0[256] = {0.0};
    s0[0] = p0[0];
    for (int i = 1; i < 256; i++)
        s0[i] = s0[i - 1] + p0[i];
//---------------------------------
    /*s[k]=求和Pn[0+..+k]*/
    /*找距离s[k]最近的像素点s[x]*/
    double minmize = 1.0;
    int min_number[256] = {0};
    double diff = 0.0;
    for (int j = 0; j < 256; j++) {
        //printf("!! == %f\n",s0[j]);
        diff = 0.0;
        minmize = 1.0;
        for (int i = 0; i < 256; i++) {
            double pix0 = 1.0 * i / 256;
            diff = fabs(s0[j] - pix0);
            //printf("%d == %f\n",i,diff);
            if (diff < minmize) {
                minmize = diff;
                min_number[j] = i;//min_number[j] --> i ;
            }

        }
        //printf("%d == %d\n",j,min_number[j]);
        //	break;
    }
    //输出
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            g[j][i] = min_number[g[j][i]];
        }
    }

    /*统计临近的相同的像素点*/
    /*计算最终的而结果并画直方图*/
}

void HistogramEqualization_b() {
    //number 统计
    long n0[256] = {0};
    /*0-255 直方图X3 RGB*/
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            n0[b[j][i]]++;
        }
    }

    /*0-1/255-...-254/255,1*/
    double p0[256] = {0.0};
    for (int i = 0; i < 256; i++) {
        p0[i] = n0[i] / (1.0 * bmpIHeader.bIHeight * bmpIHeader.bIWidth);
    }


    /*n[k]0-255:number*/

    /*Pn[n]*/
    double s0[256] = {0.0};
    s0[0] = p0[0];
    for (int i = 1; i < 256; i++)
        s0[i] = s0[i - 1] + p0[i];
//---------------------------------
    /*s[k]=求和Pn[0+..+k]*/
    /*找距离s[k]最近的像素点s[x]*/
    double minmize = 1.0;
    int min_number[256] = {0};
    double diff;
    for (int j = 0; j < 256; j++) {
        //printf("!! == %f\n",s0[j]);
        minmize = 1.0;
        for (int i = 0; i < 256; i++) {
            double pix0 = 1.0 * i / 256;
            diff = fabs(s0[j] - pix0);
            //printf("%d == %f\n",i,diff);
            if (diff < minmize) {
                minmize = diff;
                min_number[j] = i;//min_number[j] --> i ;
            }

        }
        //printf("%d == %d\n",j,min_number[j]);
        //	break;
    }
    //输出
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            b[j][i] = min_number[b[j][i]];
        }
    }

    /*统计临近的相同的像素点*/
    /*计算最终的而结果并画直方图*/
}

void HistogramEqualization_Y() {
    //number 统计
    long n0[256] = {0.0};
    /*0-255 直方图X3 RGB*/
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            n0[YuvY[j][i]]++;
        }
    }

    /*0-1/255-...-254/255,1*/
    double p0[256] = {0.0};
    for (int i = 0; i < 256; i++)
        p0[i] = n0[i] / (1.0 * bmpIHeader.bIHeight * bmpIHeader.bIWidth);

    /*n[k]0-255:number*/

    /*Pn[n]*/
    double s0[256] = {0.0};
    s0[0] = p0[0];
    for (int i = 1; i < 256; i++)
        s0[i] = s0[i - 1] + p0[i];
//---------------------------------
    /*s[k]=求和Pn[0+..+k]*/
    /*找距离s[k]最近的像素点s[x]*/
    double minmize = 1.0;
    int min_number[256] = {0};
    double diff = 0.0;
    for (int j = 0; j < 256; j++) {
        //printf("!! == %f\n",s0[j]);
        diff = 0.0;
        minmize = 1.0;
        for (int i = 0; i < 256; i++) {
            double pix0 = 1.0 * i / 256;
            diff = fabs(s0[j] - pix0);
            //printf("%d == %f\n",i,diff);
            if (diff < minmize) {
                minmize = diff;
                min_number[j] = i;//min_number[j] --> i ;
            }

        }
        //printf("%d == %d\n",j,min_number[j]);
        //	break;
    }
    //输出
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            YuvY[j][i] = min_number[YuvY[j][i]];
        }
    }

    /*统计临近的相同的像素点*/
    /*计算最终的而结果并画直方图*/
}

//--------------------------4-----------------------------
void translation(int sizeX, int sizeY, int sizeX1, int sizeY1, int x, int y) {
    int translatingMatrix[3][3] = {{1, 0, x},
                                   {0, 1, y},
                                   {0, 0, 1}};
    //int x1 = translatingMatrix[0][0]*x0+translatingMatrix[0][1]*y0+translatingMatrix[0][2];
    //int y1 = translatingMatrix[1][0]*x0+translatingMatrix[1][1]*y0+translatingMatrix[1][2];
    if (sizeX > sizeX1 || sizeY > sizeY1) {
        printf("Be careful to mentain this: sizeX<=sizeX1 and sizeY<=sizeY1.\n");
        return;
    }
    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            output_r[y0][x0] = r[y0][x0];
            output_g[y0][x0] = g[y0][x0];
            output_b[y0][x0] = b[y0][x0];
        }
    }

    for (int y0 = sizeY; y0 < sizeY1; y0++) {
        for (int x0 = sizeX; x0 < sizeX1; x0++) {
            int x1 = translatingMatrix[0][0] * x0 + translatingMatrix[0][1] * y0 + translatingMatrix[0][2];
            int y1 = translatingMatrix[1][0] * x0 + translatingMatrix[1][1] * y0 + translatingMatrix[1][2];

            output_r[y1][x1] = r[y0][x0];
            output_g[y1][x1] = g[y0][x0];
            output_b[y1][x1] = b[y0][x0];
        }
    }
    for (int y0 = sizeY + y; y0 < sizeY1 + y; y0++) {
        for (int x0 = sizeX + x; x0 < sizeX1 + x; x0++) {
            int x1 = translatingMatrix[0][0] * x0 + translatingMatrix[0][1] * y0 - translatingMatrix[0][2];
            int y1 = translatingMatrix[1][0] * x0 + translatingMatrix[1][1] * y0 - translatingMatrix[1][2];

            output_r[y1][x1] = r[y0][x0];
            output_g[y1][x1] = g[y0][x0];
            output_b[y1][x1] = b[y0][x0];
        }
    }

}

void translation1(int sizeX, int sizeY, int sizeX1, int sizeY1, int x, int y) {
    int translatingMatrix[3][3] = {{1, 0, x},
                                   {0, 1, y},
                                   {0, 0, 1}};
    //int x1 = translatingMatrix[0][0]*x0+translatingMatrix[0][1]*y0+translatingMatrix[0][2];
    //int y1 = translatingMatrix[1][0]*x0+translatingMatrix[1][1]*y0+translatingMatrix[1][2];
    if (sizeX > sizeX1 || sizeY > sizeY1) {
        printf("Be careful to mentain this: sizeX<=sizeX1 and sizeY<=sizeY1.\n");
        return;
    }
    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            output_r[y0][x0] = 255;
            output_g[y0][x0] = 255;
            output_b[y0][x0] = 255;
        }
    }

    for (int y0 = sizeY; y0 < sizeY1; y0++) {
        for (int x0 = sizeX; x0 < sizeX1; x0++) {
            int x1 = translatingMatrix[0][0] * x0 + translatingMatrix[0][1] * y0 + translatingMatrix[0][2];
            int y1 = translatingMatrix[1][0] * x0 + translatingMatrix[1][1] * y0 + translatingMatrix[1][2];

            output_r[y1][x1] = r[y0][x0];
            output_g[y1][x1] = g[y0][x0];
            output_b[y1][x1] = b[y0][x0];
        }
    }
}

void mirror(char character) {
    if (character == 'x')
        for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
            for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
                output_r[y0][x0] = r[y0][bmpIHeader.bIWidth - x0];
                output_g[y0][x0] = g[y0][bmpIHeader.bIWidth - x0];
                output_b[y0][x0] = b[y0][bmpIHeader.bIWidth - x0];
            }
        }
    else if (character == 'y')
        for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
            for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
                output_r[y0][x0] = r[bmpIHeader.bIHeight - y0][x0];
                output_g[y0][x0] = g[bmpIHeader.bIHeight - y0][x0];
                output_b[y0][x0] = b[bmpIHeader.bIHeight - y0][x0];
            }
        }
    else
        printf("something wrong with your input.");
}

void Scaling(float ratioX, float ratioY) {
    if (ratioX <= 0 || ratioY <= 0) {
        printf("Scaling paramenter wrong!\n");
        return;
    }
    if (ratioX <= 1 && ratioY <= 1) {
        for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
            for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
                int x1 = ratioX * x0;
                int y1 = ratioY * y0;

                output_r[y1][x1] = r[y0][x0];
                output_g[y1][x1] = g[y0][x0];
                output_b[y1][x1] = b[y0][x0];

            }
        }
    } else if (ratioX > 1 || ratioY > 1) {

        for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
            for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
                int x1 = ratioX * x0;
                int y1 = ratioY * y0;

                output_r[y1][x1] = r[y0][x0];
                output_g[y1][x1] = g[y0][x0];
                output_b[y1][x1] = b[y0][x0];
                records[y1][x1] = 1;
            }
        }

        for (int y0 = 0; y0 < bmpIHeader.bIHeight * ratioY; y0++) {
            for (int x0 = 0; x0 < bmpIHeader.bIWidth * ratioX; x0++) {
                if (records[y0][x0] == 0) {
                    if (ratioX > 1 && ratioY > 1) {
                        output_r[y0][x0] = r[(int) (y0 / ratioY)][(int) (x0 / ratioX)];
                        output_g[y0][x0] = g[(int) (y0 / ratioY)][(int) (x0 / ratioX)];
                        output_b[y0][x0] = b[(int) (y0 / ratioY)][(int) (x0 / ratioX)];
                    } else if (ratioX <= 1 && ratioY > 1) {
                        output_r[y0][x0] = r[(int) (y0 / ratioY)][x0];
                        output_g[y0][x0] = g[(int) (y0 / ratioY)][x0];
                        output_b[y0][x0] = b[(int) (y0 / ratioY)][x0];

                    } else if (ratioX > 1 && ratioY <= 1) {
                        output_r[y0][x0] = r[y0][(int) (x0 / ratioX)];
                        output_g[y0][x0] = g[y0][(int) (x0 / ratioX)];
                        output_b[y0][x0] = b[y0][(int) (x0 / ratioX)];

                    }
                }

            }
        }
    }

}

double Gaussian(int radial, double lamda) {
    return exp(-pow(radial, 2) / (2 * pow(lamda, 2)));
}

void RBF_Gaussian(unsigned char temple[], int x, int y, int paramenter)//3
{
    /*Paramenter is used to control how many points are included included in this process*/
    //L2 fomular
    double lamda = 0.4;//paramenter_X
    double w0 = 0.0;
    for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
        for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
            if (x0 == x && y0 == y) continue;
            double radial = abs(x - x0) + abs(y - y0);//sqrt(pow((x-x0),2)+pow((y-y0),2));
            w0 += Gaussian(radial, lamda);
        }
    }
    double pixelout[3] = {0.0};
    for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
        for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
            if (x0 == x && y0 == y)
                continue;
            double radial = abs(x - x0) + abs(y - y0);//sqrt(pow((x-x0),2)+pow((y-y0),2));
            double w1 = Gaussian(radial, lamda) / w0;
            pixelout[0] += w1 * r[y0][x0];
            pixelout[1] += w1 * g[y0][x0];
            pixelout[2] += w1 * b[y0][x0];
        }
    }
    for (int i = 0; i < 3; i++) temple[i] = (unsigned char) pixelout[i];//output!
    //printf(" %d\n",temple[0]);
}


void Rotation(double angle) {
    angle = (angle - 90) / 180 * 3.1415;
    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            //int x1 = cos(angle)*x0-sin(angle)*y0;
            //int y1 = sin(angle)*x0+cos(angle)*y0;
            int x1 = sin(angle) * (bmpIHeader.bIWidth / 2 - x0) + cos(angle) * (bmpIHeader.bIHeight / 2 - y0);
            int y1 = sin(angle) * (bmpIHeader.bIHeight / 2 - y0) - cos(angle) * (bmpIHeader.bIWidth / 2 - x0);
            y1 += bmpIHeader.bIHeight / 2;
            x1 += bmpIHeader.bIWidth / 2;
            records[y1][x1] = 1;
            output_r[y1][x1] = r[y0][x0];
            output_g[y1][x1] = g[y0][x0];
            output_b[y1][x1] = b[y0][x0];
        }
    }

    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            if (records[y0][x0] != 1) {
                /*指针传递三个参数值*/
                unsigned char temple[3];
                RBF_Gaussian(temple, x0, y0, 3);
                output_r[y0][x0] = temple[0];
                output_g[y0][x0] = temple[1];
                output_b[y0][x0] = temple[2];
            }
        }
    }
}

void Shearing(float dx, float dy) {
    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            int x1 = 0;
            int y1 = 0;
            if (dx == 0 && dy != 0) {
                x1 = x0;
                y1 = y0 + dy * x0;
            } else if (dx != 0 && dy == 0) {
                x1 = x0 + dx * y0;
                y1 = y0;
            } else if (dx != 0 && dy != 0) {
                x1 = x0 + dx * y0;
                y1 = y0 + dy * x0;
            } else
                printf("Shearing paramenter errors.\n");

            output_r[y1][x1] = r[y0][x0];
            output_g[y1][x1] = g[y0][x0];
            output_b[y1][x1] = b[y0][x0];
        }
    }

}

//------------------------------5--------------------------------
void MeanFilter(unsigned char temple[], int x, int y, int paramenter)//给出该点的准确值=1
{
    double wr = 0.0, wg = 0.0, wb = 0.0;
    int count = 0;
    for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
        for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
            if (x0 < 0 || x0 > bmpIHeader.bIWidth - 1 || y0 < 0 || y0 > bmpIHeader.bIHeight - 1)
                continue;
            wr += r[y0][x0];
            wg += g[y0][x0];
            wb += b[y0][x0];
            count++;
        }
    }
    wr /= count;
    wg /= count;
    wb /= count;
    temple[0] = (unsigned char) wr;
    temple[1] = (unsigned char) wg;
    temple[2] = (unsigned char) wb;
}

void meanFiltering(int paramenter) {

    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            unsigned char temple[3];
            MeanFilter(temple, x0, y0, paramenter);
            r[y0][x0] = temple[0];
            g[y0][x0] = temple[1];
            b[y0][x0] = temple[2];
        }
    }

    printf("Mean Filtering Success!\n");

}

void Meanfilter_Y(int paramenter) {
    for (int y = 0; y < bmpIHeader.bIHeight; y++) {
        for (int x = 0; x < bmpIHeader.bIWidth; x++) {
            double w0 = 0.0;
            int count = 0;
            for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
                for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
                    if (x0 < 0 || x0 > bmpIHeader.bIWidth - 1 || y0 < 0 || y0 > bmpIHeader.bIHeight - 1)
                        continue;
                    w0 += YuvY[y0][x0];
                    count++;
                }
            }
            YuvY[y][x] = w0 / count;
        }
    }


}

unsigned char Laplacian_filter_Y(int x, int y) {
    int paramenter = 1;
    int w0 = 0;
    for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
        for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
            if (x0 < 0 || x0 > bmpIHeader.bIWidth - 1 || y0 < 0 || y0 > bmpIHeader.bIHeight - 1)
                continue;
            if (x == x0 && y == y0) {
                w0 -= 4 * YuvY[y0][x0];
            } else if (abs(x + y - x0 - y0) == 1) {
                w0 += YuvY[y0][x0];
            }
        }
    }
    return (unsigned char) w0;
}

void LaplacianFilter_Y(double paramenter) {

    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            YuvY[y0][x0] = 0;
            YuvY[y0][x0] += abs(paramenter * Laplacian_filter_Y(x0, y0));//nomal

            YuvY[y0][x0] += YuvY[y0][x0];
        }
    }

    printf("Laplacian Filtering Success!\n");

}

//--------------------------6--------------------------------
void RBF_Bilateral(unsigned char temple[], int x, int y, int paramenter, double lamda_range, double lamda_color) {
    double w0_r = 0.0;
    double w0_g = 0.0;
    double w0_b = 0.0;
    for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
        for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
            if (x0 == x && y0 == y) continue;
            double radial = abs(x - x0) + abs(y - y0);//sqrt(pow((x-x0),2)+pow((y-y0),2));
            double color_r = abs(r[y][x] - r[y0][x0]);
            double color_g = abs(g[y][x] - g[y0][x0]);
            double color_b = abs(b[y][x] - b[y0][x0]);
            w0_r += Gaussian(radial, lamda_range) * Gaussian(color_r, lamda_color);
            w0_g += Gaussian(radial, lamda_range) * Gaussian(color_g, lamda_color);
            w0_b += Gaussian(radial, lamda_range) * Gaussian(color_b, lamda_color);
        }
    }
    double pixelout[3] = {0.0};
    for (int y0 = y - paramenter; y0 <= y + paramenter; y0++) {
        for (int x0 = x - paramenter; x0 <= x + paramenter; x0++) {
            if (x0 == x && y0 == y) continue;
            double radial = abs(x - x0) + abs(y - y0);
            double color_r = abs(r[y][x] - r[y0][x0]);
            double color_g = abs(g[y][x] - g[y0][x0]);
            double color_b = abs(b[y][x] - b[y0][x0]);
            double w1_r = Gaussian(radial, lamda_range) * Gaussian(color_r, lamda_color) / w0_r;
            double w1_g = Gaussian(radial, lamda_range) * Gaussian(color_g, lamda_color) / w0_g;
            double w1_b = Gaussian(radial, lamda_range) * Gaussian(color_b, lamda_color) / w0_b;

            pixelout[0] += w1_r * r[y0][x0];
            pixelout[1] += w1_g * g[y0][x0];
            pixelout[2] += w1_b * b[y0][x0];
        }
    }

    for (int i = 0; i < 3; i++) temple[i] = (unsigned char) pixelout[i];//output!

}

void BilateralFilter(int paramenter, double lamda_range, double lamda_color) {
    clock_t start, finish;
    start = clock();
    for (int y0 = 0; y0 < bmpIHeader.bIHeight; y0++) {
        for (int x0 = 0; x0 < bmpIHeader.bIWidth; x0++) {
            /*指针传递三个参数值*/
            unsigned char temple[3];
            RBF_Bilateral(temple, x0, y0, paramenter, lamda_range, lamda_color);
            r[y0][x0] = temple[0];
            g[y0][x0] = temple[1];
            b[y0][x0] = temple[2];
            //printf("%d\n",temple[0]-r[y0][x0]);
        }
    }
    finish = clock();
    printf("Bilateral Filter finished.\n");
    int duration = (int) (finish - start) / CLOCKS_PER_SEC;
    printf("\aIt costs %02d:%02d \n", duration / 60, duration % 60);
}

//===========================End=================================

//=====================TEST===========================
void testInputYUV() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            printf("%d ", YuvY[j][i]);
        }
    }
}

void testOutputYUV() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            printf("%d ", YuvY[j][i]);
        }
    }
}

void testRGB() {
    for (int j = 0; j < bmpIHeader.bIHeight; j++) {
        for (int i = 0; i < bmpIHeader.bIWidth; i++) {
            printf("%d %d %d \n", r[j][i], g[j][i], b[j][i]);
        }
    }
}

//=====================TEST END=================


