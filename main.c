#include <stdio.h>
#include <string.h>
#include "ProcessAPI.h"

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

    // Initialization();

    /*Standerd operations for file i/o*/
    readfile(bmpIn);
    bmpDataPart(bmpIn);


    FILE *bmpOut;
    strcpy(SourceFile_Path + strlen(SourceFile_Path) - 4, OutputFile_Down);
    bmpOut = writefile(OutputFile_Path);
    addHeadertofile(bmpIn, bmpOut);

    /* Operations */

    bmpoutput(bmpOut);

    fclose(bmpIn);
    fclose(bmpOut);

    return 0;
}
