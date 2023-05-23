#include "cmprss.h"

uint X_SIZE = 256;
uint Y_SIZE = 70;


char ascii_grayscale1[9] = {'.', ':', '-', '=', '+', '*', '#', '%', '@'};
char ascii_grayscale2[90] = {'.', '-', '\'', ':', '_', ',', '^', '=', ';', '>', '<', '+', '!', 'r', 'c', '*', '/', 'z', '?' ,'s', 'L', 'T', 'v', ')', 'J', '7', '(', '|', 'F','i', '{', 'C', '}', 'f', 'I', '3', '1', 't', 'l', 'u', '[', 'n', 'e', 'o', 'Z', '5', 'Y', 'x', 'j', 'y', 'a', ']', '2', 'E', 'S', 'w', 'q', 'k', 'P', '6', 'h', '9', 'd', '4', 'V', 'p', 'O', 'G', 'b', 'U', 'A', 'K', 'X', 'H', 'm', '8', 'R', 'D', '#', '$', 'B', 'g', '0', 'M', 'N', 'W', 'Q', '%', '&', '@'};

static float lerp(float s, float e, float t) {
    return s+(e-s)*t;
}

static float blerp(float p00, float p01, float p10, float p11, float tx, float ty) {
    return lerp(lerp(p00, p10, tx), lerp(p01, p11, tx), ty);
}

void compress_frame(char *bitmap, uchar *img, int wrap, int xsize, int ysize) {
    int x, y;

    for (x=0, y=0; y<Y_SIZE; x++) {
        if (x>=X_SIZE) {
            x=0; y++;
            if (y>=Y_SIZE) break;
        }

        
        float gx = x/(float) X_SIZE*(xsize-1);
        float gy = y/(float)Y_SIZE*(ysize-1);
        int gxi = (int) gx, gyi = (int) gy;

        uint32_t res = 0;
        uint32_t p00 = img[gyi*wrap+gxi];
        uint32_t p01 = img[gyi*wrap+(gxi+1)];
        uint32_t p10 = img[(gyi+1)*wrap+gxi];
        uint32_t p11 = img[(gyi+1)*wrap+(gxi+1)];

        uint8_t i;
        for (i=0; i<3; i++)
            res |= (uint8_t)blerp(getbyte(p00, i), getbyte(p01, i), getbyte(p10, i), getbyte(p11, i), gy-gyi, gx-gxi) << (8*i);

        bitmap[X_SIZE*y+x] = ascii_grayscale1[(res*9)/256];
    }
}
