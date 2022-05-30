//
//  bmp.c
//  bmp2jpeg
//
//  Created by ubuntu-vm on 2014. 11. 11..
//  Copyright (c) 2014??Jaehwan Lee. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bmp.h"


void init_bmp_context (bmp_context_t* context, const char* source)
{
	uint32_t i, j;
	FILE* fp = NULL;
	bmp_header_t header;

	// open file
	if ((fp = fopen (source, "r")) == NULL) {
		fprintf (stderr, "cannot open %s for read\n", source);
		exit (1);
 	}

	// read header
	memset (&header, 0x00, sizeof (bmp_header_t));
	fread (&header.signature, 1, BMP_HEADER_SIZE, fp);
	context->header = header;

	// check size
	/*
	if (header.bmp_width != 512 || header.bmp_height != 512) {
		fprintf (stderr, "bitmap must be 512x512\n");
		exit (1);
	}
	*/

	// check color components
	if (header.bpp != 24) {
		fprintf (stderr, "bitmap must be 24bit\n");
		exit (1);
	}

	// print info
	printf ("%s: %ux%u %ubpp\n", source, header.bmp_width, 
		header.bmp_height, header.bpp);

	// init context
    uint32_t width = header.bmp_width;
    uint32_t height = header.bmp_height;

    context->channel_red = (uint8_t*)malloc (width * height);
    context->channel_green = (uint8_t*)malloc (width * height);
    context->channel_blue = (uint8_t*)malloc (width * height);

	// read rgb channels
	uint8_t* scanline = (uint8_t*)malloc (width * 3);
	for (i=0; i<height; i++) {
		// 
		uint32_t h_idx = (height - i - 1) * width;

		// read a scanline of rgb components
		fread (scanline, 1, width * 3, fp);

		// extract rgb components
		for (j=0; j<width; j++) {
			context->channel_blue[h_idx + j] = scanline[j * 3];
			context->channel_green[h_idx + j] = scanline[j * 3 + 1];
			context->channel_red[h_idx + j] = scanline[j * 3 + 2];
		}
	}

	// release resources
	free (scanline);
	fclose (fp);

	printf ("bmp read done.\n");
}

void deinit_bmp_context (bmp_context_t* context)
{
	free (context->channel_red);
	context->channel_red = NULL;
	free (context->channel_green);
	context->channel_green = NULL;
	free (context->channel_blue);
	context->channel_blue = NULL;
}


static uint8_t rgb_to_gray (uint8_t r, uint8_t g, uint8_t b)
{
	return (uint8_t)((0.2126f * (float)r +
					0.7152f * (float)g + 
					0.0722f * (float)b));
}

void bmp_write_grayscale (bmp_context_t* context, const char* dest)
{
	int i, j;
	FILE* fp = NULL;

	// open file
	if ((fp = fopen (dest, "w")) == NULL) {
		fprintf (stderr, "cannot open %s for write\n", dest);
		exit (1);
 	}

 	// write header
 	fwrite (&context->header.signature, 1, BMP_HEADER_SIZE, fp);

	// compute and write grayscale rgb channels
	uint32_t width = context->header.bmp_width;
	uint32_t height = context->header.bmp_height;

	uint8_t* scanline = (uint8_t*)malloc (width * 3);
	for (i=0; i<height; i++) {
		// from bottom to top
		uint32_t h_idx = (height - i - 1) * width;

		// compute gray for scanline
		for (j=0; j<width; j++) {
			uint8_t gray =  rgb_to_gray (context->channel_red[h_idx + j],
										context->channel_green[h_idx + j],
										context->channel_blue[h_idx + j]);
			scanline[j * 3] = scanline[j * 3 + 1] = scanline[j * 3 + 2] = gray;
		}

		// write line
		fwrite (scanline, 1, width * 3, fp);
	}
	
	// release resources
	fclose (fp);
	free (scanline);
}

void bmp_convert_grayscale (bmp_context_t* context)
{
	uint32_t i, j;
	uint32_t width = context->header.bmp_width;
	uint32_t height = context->header.bmp_height;

	for (i=0; i<height; i++) {

		uint32_t h_idx = (height - i - 1) * width;

		for (j=0; j<width; j++) {
			uint8_t gray = rgb_to_gray (context->channel_red[h_idx + j],
										context->channel_green[h_idx + j],
										context->channel_blue[h_idx + j]);

			context->channel_red[h_idx + j] = gray;
			context->channel_green[h_idx + j] = gray;
			context->channel_blue[h_idx + j] = gray;
		}
	}
}