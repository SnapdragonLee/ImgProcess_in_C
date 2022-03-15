#include "header.h"

char *SourceFile_Path, *OutputFile_Path;
const char *OutputFile_Down = "-output.bmp";

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

    readfile(bmpIn);
    bmpDataPart(bmpIn);


    strcpy(SourceFile_Path + strlen(SourceFile_Path) - 4, OutputFile_Down);
    FILE *bmpOut = fopen(OutputFile_Path, "wb+");
    if (bmpOut == NULL) {
        printf("%s Open failed!\n", SourceFile_Path);
        exit(1);
    }

    writefile(bmpOut);

    fclose(bmpIn);
    fclose(bmpOut);

    return 0;
}
