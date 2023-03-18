#pragma once
#include <time.h>
#include <stdlib.h>

#define NANO 1000000000.0f
typedef unsigned int uint;

uint time_delta_ns(struct timespec from, struct timespec to);

double tv_diff(struct timespec from, struct timespec to);

void get_sleep_time(struct timespec from, struct timespec to, struct timespec *tw, uint FPS);
