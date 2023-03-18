#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define getbyte(n, x) (n>>(x*8) & 0xFF)

typedef unsigned int uint;
typedef unsigned short int usint;
typedef unsigned char uchar;


float lerp(float s, float e, float t);

float blerp(float p00, float p01, float p10, float p11, float tx, float ty);

void compress_frame(uint* bitmap, uchar* img, int wrap, int xsize, int ysize);
