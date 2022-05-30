#ifndef __jpegenc_h__
#define __jpegenc_h__

#include "bmp.h"

// -------------------------------------------------------------------------
// case 1 experiments: 0 <= x,y,u,v <= 512	whole image
// case 2 experiments: 0 <= x,y,u,v <= 8	each 8x8 block
// -------------------------------------------------------------------------
//#define EXPERIMENT_CASE_1
#define EXPERIMENT_CASE_2

// -------------------------------------------------------------------------
// experiment 1: along u axis
// experiment 2: along v axis
// experiment 3: along u and v axis
// -------------------------------------------------------------------------
//#define EXPERIMENT_NUM_1
//#define EXPERIMENT_NUM_2
#define EXPERIMENT_NUM_3


int jpeg_exp_write (bmp_context_t* context, const char* dest, int step);

#endif //__jpegenc_h__