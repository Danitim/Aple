#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "deps/cmprss.h"
#include "deps/timecalc.h"
#include "deps/cmds.h"

uint FC, FPS = 60;
uint TPF;
ush X=128, Y=64;

char *buf;

static int decode_packet(char *bitmap, AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
static void print_bitmap(char *bitmap);

int main(int argc, const char *argv[]) {
    if (argc < 2) { printf("You need to specify a media file.\n"); return -1;}
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) return -1;
    if (avformat_open_input(&pFormatContext, argv[1], NULL, NULL) != 0) return -1;
    if (avformat_find_stream_info(pFormatContext,  NULL) < 0) return -1;
    AVCodec *pCodec = NULL;
    AVCodecParameters *pCodecParameters =  NULL;
    int video_stream_index = -1;

    buf = malloc(1024*1024);  
    FC = get_frame_count(argv[1]);
    FPS = get_frame_rate(argv[1]);

    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        AVCodec *pLocalCodec = NULL;
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec==NULL) continue;

        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
          if (video_stream_index == -1) {
            video_stream_index = i;
            pCodec = pLocalCodec;
            pCodecParameters = pLocalCodecParameters;
          }
        }
    }

    if (video_stream_index == -1) return -1;
    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if (!pCodecContext) return -1;
    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0) return -1;
    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0) return -1;
    AVFrame *pFrame = av_frame_alloc();
    if (!pFrame) return -1;
    AVPacket *pPacket = av_packet_alloc();
    if (!pPacket) return -1;

    int response = 0;
    TPF = NANO/FPS;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    X = w.ws_col; Y = w.ws_row;
    X_SIZE = X; Y_SIZE = Y;

    char* bitmap = (char*) calloc(X_SIZE*Y_SIZE, sizeof(char));
    struct timespec last, cur;
    clock_gettime(CLOCK_MONOTONIC_RAW, &last);


    printf("\e[?25l");
    while (1) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &cur);
        if (time_delta_ns(last, cur) >= TPF) {
            while(av_read_frame(pFormatContext, pPacket) >= 0)
                if (pPacket->stream_index == video_stream_index) break;

            response = decode_packet(bitmap, pPacket, pCodecContext, pFrame);
            if (response < 0) break;

            print_bitmap(bitmap);
            last = cur;
            av_packet_unref(pPacket);
        }
    }

    avformat_close_input(&pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);
    free(buf);
    return 0;
}

static int decode_packet(char *bitmap, AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame) {
    int response = avcodec_send_packet(pCodecContext, pPacket);
    if (response < 0) { return response;}
    while (response >= 0) {
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            break;
        } else if (response < 0) return response;

        if (response >= 0)
            compress_frame(bitmap, pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height);
    }
    return 0;
}

const ush offset = 7;

static void print_bitmap(char *bitmap) {
    strcpy(buf, "\x1B[?25h");
    strcpy(buf+1, "\033[1;1H");
    for (int y=0; y<Y; y++) {
        for (int x=0; x<X; x++) {
            buf[y*X+x+offset] = bitmap[X_SIZE*y+x];
        }
    }
    fwrite(buf, sizeof(char), X*Y+offset, stdout);
    fsync(1);
}
