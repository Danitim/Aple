#include "cmprss.h"

float lerp(float s, float e, float t) {
    return s+(e-s)*t;
}

float blerp(float p00, float p01, float p10, float p11, float tx, float ty) {
    return lerp(lerp(p00, p10, tx), lerp(p01, p11, tx), ty);
}

void compress_frame(uint *bitmap, uchar *img, int wrap, int xsize, int ysize) {
    int x, y;

    for (x=0, y=0; y<64; x++) {
        if (x>=128) {
            x=0; y++;
            if (y>=64) break;
        }

        
        float gx = x/128.0*(xsize-1);
        float gy = y/64.0*(ysize-1);
        int gxi = (int) gx, gyi = (int) gy;

        uint32_t res = 0;
        uint32_t p00 = img[gyi*wrap+gxi];
        uint32_t p01 = img[gyi*wrap+(gxi+1)];
        uint32_t p10 = img[(gyi+1)*wrap+gxi];
        uint32_t p11 = img[(gyi+1)*wrap+(gxi+1)];

        uint8_t i;
        for (i=0; i<3; i++)
            res |= (uint8_t)blerp(getbyte(p00, i), getbyte(p01, i), getbyte(p10, i), getbyte(p11, i), gy-gyi, gx-gxi) << (8*i);
        
        if (res>=128)
            bitmap[(128*y+x)/32] |= 1UL << (31-x%32);
        else
            bitmap[(128*y+x)/32] &= ~(1UL << (31-x%32));

    }
}
