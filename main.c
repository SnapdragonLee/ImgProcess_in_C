#include <stdio.h>
#include "ImageAPI.h"

int main(int argc, char *argv[]) {
    /* Open bmp file */
    unsigned char *fp_temp;

    FILE *bmpin;
    FILE *bmpout;
    Initialization();

//===============first picture -- Bilateral Filter==========

    /*Standerd operations for file i/o*/
    bmpin = openfile(SourceFile);
    bmpDataPart(bmpin);

    bmpout = writefile(OutputFile1);
    addHeadertofile(bmpin, bmpout);

    /*your operations for your picture*/

    bmpoutput(bmpout);

    /*Standerd operations for close*/
    fclose(bmpin);
    fclose(bmpout);

    return 0;
}
