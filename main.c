#include <stdio.h>
#include "ImageAPI.h"

#define SourceFile "../lena2.bmp"
#define OutputFile1 "../lena2-output.bmp"
// #define OutputFile2 "C:\\Users\\Administrator\\Desktop\\air2.bmp"
// #define OutputFile3 "C:\\Users\\Administrator\\Desktop\\air3.bmp"
// #define OutputFile4 "C:\\Users\\Administrator\\Desktop\\air4.bmp"
// #define OutputFile5 "C:\\Users\\Administrator\\Desktop\\air5.bmp"
// #define OutputFile6 "C:\\Users\\Administrator\\Desktop\\air6.bmp"
// #define OutputFile7 "C:\\Users\\Administrator\\Desktop\\air7.bmp"

int main(int argc, char *argv[]) {
    FILE *bmpIn = fopen(SourceFile, "rb");

    if (bmpIn == NULL) {
        printf("Open failed! No file named %s\n", SourceFile);
        exit(1);
    }

    FILE *bmpOut;
    // Initialization();

    /*Standerd operations for file i/o*/
    openfile(bmpIn);
    bmpDataPart(bmpIn);

    bmpOut = writefile(OutputFile1);
    addHeadertofile(bmpIn, bmpOut);

    /* Operations */

    bmpoutput(bmpOut);

    fclose(bmpIn);
    fclose(bmpOut);

    return 0;
}
