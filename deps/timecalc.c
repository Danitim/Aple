#include "timecalc.h"

uint time_delta_ns(struct timespec from, struct timespec to) {
    return abs((to.tv_sec - from.tv_sec)*NANO + (to.tv_nsec - from.tv_nsec));    
}

double tv_diff(struct timespec from, struct timespec to) {
    return (to.tv_sec - from.tv_sec) + (to.tv_nsec - from.tv_nsec)/NANO;
}

void get_sleep_time(struct timespec from, struct timespec to, struct timespec *tw, uint FPS) {
    double num = 1.0/FPS - tv_diff(from, to);
    tw->tv_sec = (time_t) num > 0 ? (time_t) num : 0;
    tw->tv_nsec = (long) ((num - (int) num)*NANO) > 0 ? (long) ((num - (int) num)*NANO) : 0;
    return;
}
