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

    RGB2Gray();

    /*RGB2YUV();
    YUV2Gray();
    YUV2RGB();
    HistogramEqualization_b();
    HistogramEqualization_g();
    HistogramEqualization_r();*/

    /******* Add Img Process Algorithm upon *******/

    strcpy(SourceFile_Path + strlen(SourceFile_Path) - 4, OutputFile_Down);
    FILE *bmpOut = fopen(OutputFile_Path, "wb+");
    if (bmpOut == NULL) {
        printf("%s Open failed!\n", SourceFile_Path);
        exit(1);
    }

    headerChange(8, 512, 512);

    writeFile(bmpOut);

    fclose(bmpIn);
    fclose(bmpOut);
    return 0;
}
