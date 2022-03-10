#include <stdio.h>
#include "ImageAPI.h"

int main(int argc, char *argv[]) {
    /* Open bmp file */
    unsigned char *fp_temp;

    FILE *fpbmp;
    FILE *fpout;
    Initialization();

//===============first picture -- Bilateral Filter==========

    /*Standerd operations for file i/o*/
    fpbmp = openfile(SourceFile);
    bmpDataPart(fpbmp);

    fpout = writefile(OutputFile1);
    addHeadertofile(fpbmp, fpout);

    /*your operations for your picture*/

    bmpoutput(fpout);

    /*Standerd operations for close*/
    fclose(fpbmp);
    fclose(fpout);

    return 0;
}
