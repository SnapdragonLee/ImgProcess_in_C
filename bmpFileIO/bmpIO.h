//
// Created by SNAPDRAGON_11800H on 3/11/2022.
//

#ifndef IMGPROCESS_IN_C_BMPIO_H
#define IMGPROCESS_IN_C_BMPIO_H

void readFile(FILE *bmpFileIn);

void bmpFHeaderRead(FILE *bmpFileIn);

void bmpIHeaderRead(FILE *bmpFileIn);

void bmpDataFrame(FILE *bmpFileIn);

void writeFile(FILE *bmpFileOut);

void bmpOutput(FILE *bmpFileOut);

void addHeader(FILE *bmpFileOut);





#endif //IMGPROCESS_IN_C_BMPIO_H
