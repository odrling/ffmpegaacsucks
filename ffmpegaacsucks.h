#ifndef FFMPEGAACSUCKS_H
#define FFMPEGAACSUCKS_H 1

#include <libavformat/avformat.h>
#include <stdbool.h>

#define FFMPEGAACSUCKS_DETECTED 1
#define FFMPEGAACSUCKS_UNDETECTED 0
#define FFMPEGAACSUCKS_FAILURE -1

struct ffaacsucks_result {
  int n_streams;
  int *streams;
};

struct ffaacsucks_result *ffaacsucks_check(char *filepath);

// NOTE: ffaacsucks_check_avfcontext will read frames from aac stream and discard other streams
// which would work poorly with other uses of the avf context
struct ffaacsucks_result *ffaacsucks_check_avfcontext(AVFormatContext *s, char *filepath);

bool ffaacsucks_check_aac_stream_packet(AVPacket *pkt, char *filepath);

void ffaacsucks_result_free(struct ffaacsucks_result *res);

#endif
