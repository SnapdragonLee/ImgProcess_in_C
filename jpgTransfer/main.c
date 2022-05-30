#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bmp.h"
#include "jpegenc.h"
#include "jpeg_exp.h"

int main(int argc, char **argv) {
    bmp_context_t bmp;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s bitmap_image.bmp\n", argv[0]);
        return 1;
    }

    init_bmp_context(&bmp, argv[1]);

    //bmp_write_grayscale (&bmp, "grayout.bmp");
    //bmp_convert_grayscale (&bmp);
    //jpeg_write_grayscale (&bmp, "grayout.jpg");

    // experiment code
    printf("generating results...\n");
    int step;
    char destpath[64];
    for (step = 0; step < 8; step++) {
        memset(destpath, 0x00, 64);
        sprintf(destpath, "result_step_%d.jpg", step);
        jpeg_exp_write(&bmp, destpath, step);
    }

    deinit_bmp_context(&bmp);

    return 0;
}
