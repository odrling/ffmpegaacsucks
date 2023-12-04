#include <libavcodec/codec_id.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffmpegaacsucks.h"

#define FFAACSUCKS_LAVC_SIGNATURE "Lavc"

#define NOT_FOUND_STR "no Lavc/FFmpeg AAC stream was found\n"

struct ffaacsucks_result *ffaacsucks_check(char *filepath) {
  AVFormatContext *s = NULL;
  unsigned int i, aac_streams;
  int ret, pkt_type, namelen, skip;
  struct ffaacsucks_result *res = malloc(sizeof(struct ffaacsucks_result));
  res->n_streams = 0;
  res->streams = NULL;

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

    aac_streams--;

    uint8_t b = pkt->buf->data[0];
    pkt_type = (b & 0xe0) >> 5;
    if (pkt_type != 6) {
      fprintf(stderr, "unexpected packet type found in stream %d (%d)\n",
              pkt->stream_index, pkt_type);
      goto next_pkt;
    }

    namelen = (b & 0x1e) >> 1;

    if (namelen == 15)
      skip = 3;
    else
      skip = 2;

    char *comment = (char *)pkt->buf->data + skip;
    int cmp = strncmp(comment, FFAACSUCKS_LAVC_SIGNATURE, 4);
    if (cmp == 0) {
      res->streams[res->n_streams++] = pkt->stream_index;
    }

next_pkt:
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
  if (res->streams != NULL)
    free(res->streams);
  free(res);
}
