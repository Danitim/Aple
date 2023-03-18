#include "cmds.h"

uint get_frame_count(const char *file_name) {
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

uint get_frame_rate(const char *file_name) {
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
