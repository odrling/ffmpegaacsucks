#include <libavcodec/codec_id.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libffmpegaacsucks.h"

#define NOT_FOUND_STR "no Lavc/FFmpeg AAC stream was found\n"

struct ffaacsucks_result *ffaacsucks_check(char *filepath) {
  AVFormatContext *s = NULL;
  unsigned int i, aac_streams;
  int ret;
  struct ffaacsucks_result *res = malloc(sizeof(struct ffaacsucks_result));
  res->n_streams = 0;

  ret = avformat_open_input(&s, filepath, NULL, NULL);

  if (ret < 0) {
    perror("failed to load file");
    res->n_streams = -1;
    return res;
  }
  aac_streams = 0;

  for (i = 0; i < s->nb_streams; i++) {
    AVStream *st = s->streams[i];

    if (st->codecpar->codec_id == AV_CODEC_ID_AAC) {
      aac_streams++;
    } else {
      st->discard = AVDISCARD_ALL;
    }
  }

  if (aac_streams == 0)
    goto free_avf;

  res->streams = malloc(sizeof(int) * aac_streams);
  AVPacket *pkt = av_packet_alloc();

  while (aac_streams > 0) {
    ret = av_read_frame(s, pkt);
    if (ret < 0)
      perror("av_read_frame");

    // skip first 3 bytes
    char *comment = (char *)pkt->buf->data + 3;
    int cmp = strncmp(comment, "Lavc", 4);
    aac_streams--;
    if (cmp == 0) {
      res->streams[res->n_streams++] = pkt->stream_index;
    }
    s->streams[pkt->stream_index]->discard = AVDISCARD_ALL;

    av_packet_unref(pkt);
  }

  av_packet_free(&pkt);

free_avf:
  avformat_close_input(&s);
  avformat_free_context(s);

  return res;
}

void ffaacsucks_result_free(struct ffaacsucks_result *res) {
  free(res->streams);
  free(res);
}
