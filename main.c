//
// Created by SNAPDRAGON_11800H on 3/12/2022.
//

#include "header.h"

#pragma GCC optimize ("O3")

int main(int argc, char *argv[]) {
    if (argc > 1) {
        SourceFile_Path = OutputFile_Path = argv[1];
    } else {
        printf("Please input a file path:\n");
    }

    FILE *In = fopen(SourceFile_Path, "rb");
    if (In == NULL) {
        printf("Open failed! No file named %s\n", SourceFile_Path);
        exit(1);
    }

    readBMPFile(In);
    bmpDataFrame(In);

    /******* Add Img Process Algorithm below *******/
    //LinearTransform(gray);

    //RGB2Gray(r, g, b, gray);
    //RGB2YUV(r, g, b, y, u, v);
    //YUV2Gray(y, u, v, gray);
    //YUV2RGB(y, u, v, r, g, b);
    //histogramEqualization24(r, g, b);
    //histogramEqualization8(gray);

    //dctinit(bmpIHeader.bIHeight, bmpIHeader.bIWidth);
    //dct2(gray, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth);
    //idct2(gray, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth);

    // fftinit(bmpIHeader.bIHeight, bmpIHeader.bIWidth);
    // fft2(gray, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth);


    //fft2_freq(r, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth, 150);
    //fft2_freq(g, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth, 150);
    //fft2_freq(b, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth, 150);


    //fft2_freq(gray, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth, 200);

    //ifft2(gray, dat_fl, bmpIHeader.bIHeight, bmpIHeader.bIWidth);

    //linearTransform8(r, g, b, gray);

    /******* Add Img Process Algorithm upon *******/

    strcpy(SourceFile_Path + strlen(SourceFile_Path) - 4, "-output.jpg");
    FILE *Out = fopen(OutputFile_Path, "wb+");
    if (Out == NULL) {
        printf("%s Open failed!\n", SourceFile_Path);
        exit(1);
    }

    // headerChange(8, 512, 512);
    // output will get gray metrix when activate headerChange, otherwise get rgb metrix

    // writeBMPFile(Out);
    jpeg_exp_write(Out, 100, 7);


    fclose(In);
    fclose(Out);
    return 0;
}
