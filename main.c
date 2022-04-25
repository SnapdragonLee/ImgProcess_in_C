//
// Created by SNAPDRAGON_11800H on 3/12/2022.
//

#include "header.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        SourceFile_Path = OutputFile_Path = argv[1];
    } else {
        printf("Please input a file path:\n");
    }

    FILE *bmpIn = fopen(SourceFile_Path, "rb");
    if (bmpIn == NULL) {
        printf("Open failed! No file named %s\n", SourceFile_Path);
        exit(1);
    }

    readFile(bmpIn);
    bmpDataFrame(bmpIn);

    /******* Add Img Process Algorithm below *******/
    //LinearTransform(gray);

    //RGB2YUV(r, g, b, y, u, v);
    //YUV2Gray(y, u, v, gray);
    //YUV2RGB(y, u, v, r, g, b);
    //histogramEqualization24(r, g, b);
    //histogramEqualization8(gray);
    //linearTransform8(r, g, b, gray);

    /******* Add Img Process Algorithm upon *******/

    strcpy(SourceFile_Path + strlen(SourceFile_Path) - 4, OutputFile_Down);
    FILE *bmpOut = fopen(OutputFile_Path, "wb+");
    if (bmpOut == NULL) {
        printf("%s Open failed!\n", SourceFile_Path);
        exit(1);
    }

    headerChange(8, 512, 512);
    // output will get gray metrix when activate headerChange, otherwise get rgb metrix


    writeFile(bmpOut);

    fclose(bmpIn);
    fclose(bmpOut);
    return 0;
}
