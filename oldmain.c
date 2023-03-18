#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "codec.h"

const int FPS = 10;
const double WRITE_TIME = 0.05;

#define NANO 1000000000.0f
#define u_ll unsigned long long

double tv_diff(struct timespec from, struct timespec to) {
    return (to.tv_sec - from.tv_sec) + (to.tv_nsec - from.tv_nsec)/NANO;
}

void get_sleep_time(struct timespec from, struct timespec to, struct timespec *tw) {
    double num = 1.0/FPS - tv_diff(from, to);
    tw->tv_sec = (time_t) num > 0 ? (time_t) num : 0;
    tw->tv_nsec = (long) ((num - (int) num)*NANO) > 0 ? (long) ((num - (int) num)*NANO) : 0;
    return;
}

void set_timespec(double time, struct timespec *t) {
    t->tv_sec = (time_t) time > 0 ? (time_t) time : 0;
    t->tv_nsec = (long) ((time - (int) time)*NANO) > 0 ? (long) ((time - (int) time)*NANO) : 0;
    return;
}

uint get_frame_count(char *file_name) {
    char buf[256];
    sprintf(buf, "ffmpeg -i %s -map 0:v:0 -c copy -f null -y /dev/null 2>&1 | grep -Eo 'frame= *[0-9]+ *' | grep -Eo '[0-9]+' | tail -1", file_name); 

    FILE *f;
    f = popen(buf, "r");

    if (f == NULL) {pclose(f); return 0;}
    
    uint count = 0;
    fscanf(f, "%u", &count);

    pclose(f);
    return count;
}

uint get_frame_rate(char *file_name) {
    char buf[256];
    sprintf(buf, "ffprobe -v error -select_streams v -of default=noprint_wrappers=1:nokey=1 -show_entries stream=r_frame_rate %s", file_name);

    FILE *f;
    f = popen(buf, "r");

    if (f == NULL) {pclose(f); return 0;}

    uint rate = 0;
    fscanf(f, "%u", &rate);

    pclose(f);
    return rate;
}

void extract_frame(char *file_name, int index, double koef) {

    char time_buf[32];
    sprintf(time_buf, "00:%02lld:%02lld.%03lld", ((u_ll) (index*koef/60))%60, ((u_ll) (index*koef))%60, ((u_ll) (index*koef*1000))%1000);
    //printf("Current frame time: %s\n", time_buf);

    char buf[256];
    sprintf(buf, "ffmpeg -y -loglevel quiet -ss %s -i %s -r %d -frames:v 1 -q:v 2 frame.bmp", time_buf, file_name, FPS);

    FILE *f;
    f = popen(buf, "r");
    if (f == NULL) {pclose(f); return;}

    return;
}

void clr_csl() {
    FILE *f;
    f = popen("clear", "r");
    pclose(f);
    return;
}

int main() {
    struct timespec t1, t2, tm, ts, tw;
    set_timespec(WRITE_TIME, &ts);
    uint bitmap[256] = {0};
    char *file_name = "test.mp4";
    
    uint old_frame_count = get_frame_count(file_name);
    uint frame_rate = get_frame_rate(file_name);
    uint frame_count = old_frame_count*FPS/frame_rate;
    double koef = (double) old_frame_count / ((double) frame_count * (double) frame_rate);

    //printf("Transmutation coefficient: %f\n", koef);
    //printf("%d -> %d   =>   %d -> %d\n", frame_rate, FPS, old_frame_count, frame_count);

    for (int n=0; n<frame_count; n++) {
        //system("clear");
        clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
        extract_frame(file_name, n, koef);
        clock_gettime(CLOCK_MONOTONIC_RAW, &tm);
        nanosleep(&ts, NULL);
        get_bitmap(bitmap);
        for (int i=0; i<256; i++) {
            for (int j=0; j<32; j++) {
                printf(bitmap[i] & (1 << (31-j)) ? "@" : ".");
                bitmap[i] &= ~(1 << (31-j));
            }
            if (i%4==3) printf("\n");
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &t2);
        get_sleep_time(t1, t2, &tw);
        printf("Extraction: %f ; Bitmap: %f ; Time to sleep = %f\n", tv_diff(t1, tm), tv_diff(tm, t2), 1/FPS-tv_diff(t1, t2));
        nanosleep(&tw, NULL);
    }

    return 0;
}
