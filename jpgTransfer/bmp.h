//
//  bmp.h
//  bmp2jpeg
//
//  abstract: read 512 x 512 pixels of 24bit bitmap picture
// 
//  Created by ubuntu-vm on 2014. 11. 11..
//  Copyright (c) 2014??Jaehwan Lee. All rights reserved.
//

#ifndef __bmp_h__
#define __bmp_h__

#include <stdint.h>

#define BMP_HEADER_SIZE 54
typedef struct __bmp_header {

	uint16_t padding;
	uint16_t signature;
	uint32_t bmp_file_size;
	uint16_t reserve_1;
	uint16_t reserve_2;
	uint32_t soi_offset;
	uint32_t bmp_header_size;
	uint32_t bmp_width;
	uint32_t bmp_height;	
	uint16_t number_of_planes;
	uint16_t bpp;
	uint32_t compression_type;
	uint32_t img_data_size;
	uint32_t horizontal_ppm;
	uint32_t vertical_ppm;
	uint32_t number_of_colors;
	uint32_t number_of_important_colors;

} bmp_header_t;

typedef struct __image_stream {

	bmp_header_t header;

	uint8_t* channel_red;
	uint8_t* channel_green;
	uint8_t* channel_blue;

} bmp_context_t;

// read from bmp source
void init_bmp_context (bmp_context_t* context, const char* source);
void deinit_bmp_context (bmp_context_t* context);

// save to bmp grayscale image
void bmp_write_grayscale (bmp_context_t* context, const char* dest);

// convert rgb channels to grayscale
void bmp_convert_grayscale (bmp_context_t* context);

#endif //__bmp_h__