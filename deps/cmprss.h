#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define getbyte(n, x) (n>>(x*8) & 0xFF)

extern unsigned int X_SIZE;
extern unsigned int Y_SIZE;

typedef unsigned int uint;
typedef unsigned short int usint;
typedef unsigned char uchar;

void compress_frame(char* bitmap, uchar* img, int wrap, int xsize, int ysize);
