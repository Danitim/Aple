#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>

#include "cmprss.h"
#include "timecalc.h"
#include "cmds.h"

uint OFC, OFPS;
uint FC, FPS = 15;
uint TPF;

static int decode_packet(uint *bitmap, AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
static void print_bitmap(uint *bitmap);

int main(int argc, const char *argv[]) {
    if (argc < 2) { printf("You need to specify a media file.\n"); return -1;}
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (!pFormatContext) { return -1;}
    if (avformat_open_input(&pFormatContext, argv[1], NULL, NULL) != 0) { return -1;}
    if (avformat_find_stream_info(pFormatContext,  NULL) < 0) { return -1;}
    AVCodec *pCodec = NULL;
    AVCodecParameters *pCodecParameters =  NULL;
    int video_stream_index = -1;
      
    OFC = get_frame_count(argv[1]);
    OFPS = get_frame_rate(argv[1]);

    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        AVCodecParameters *pLocalCodecParameters =  NULL;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        AVCodec *pLocalCodec = NULL;
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        if (pLocalCodec==NULL) { continue;}

        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
          if (video_stream_index == -1) {
            video_stream_index = i;
            pCodec = pLocalCodec;
            pCodecParameters = pLocalCodecParameters;
          }
        }
    }

    if (video_stream_index == -1) { return -1;}
    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if (!pCodecContext){ return -1;}
    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0) { return -1;}
    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0) { return -1;}
    AVFrame *pFrame = av_frame_alloc();
    if (!pFrame) { return -1;}
    AVPacket *pPacket = av_packet_alloc();
    if (!pPacket) { return -1; }

    int response = 0;
    int how_many_packets_to_process = 2;
    TPF = NANO/OFPS;

    uint bitmap[256] = {0};
    struct timespec last, cur;
    clock_gettime(CLOCK_MONOTONIC_RAW, &last);

    //printf("Current frame rate: %u\n", OFPS);
    while (1) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &cur);
        //printf("Time delta: %u/%u\n", time_delta_ns(last, cur), TPF);
        if (time_delta_ns(last, cur) >= TPF) {
            if (av_read_frame(pFormatContext, pPacket) >= 0) {
                if (pPacket->stream_index == video_stream_index) {
                    response = decode_packet(bitmap, pPacket, pCodecContext, pFrame);
                    if (response < 0) break;
                    //if (--how_many_packets_to_process <= 0) break;
                }
                av_packet_unref(pPacket);
                print_bitmap(bitmap);
                last = cur;
            }
        }
        sleep(0.002);
    }


    avformat_close_input(&pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);
    return 0;
}

static int decode_packet(uint *bitmap, AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame)
{
  int response = avcodec_send_packet(pCodecContext, pPacket);

  if (response < 0) { return response;}

  while (response >= 0)
  {
    response = avcodec_receive_frame(pCodecContext, pFrame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
      break;
    } else if (response < 0) { return response;}

    if (response >= 0) {
      compress_frame(bitmap, pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height);
    }
  }
  return 0;
}

static void print_bitmap(uint *bitmap) {
    for (int x=0; x<256; x++) {
          for (int y=0; y<32; y++) {
              printf(bitmap[x] & (1 << (31 - y)) ? "@" : ".");
          }
          bitmap[x] = 0;
          if (x%4==3) printf("\n");
      }
    printf("\n\n\n");
}
