#include <libavcodec/codec_id.h>
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffmpegaacsucks.h"
#include "version.h"

#define FFAACSUCKS_LAVC_SIGNATURE "Lavc"

#define NOT_FOUND_STR "no Lavc/FFmpeg AAC stream was found\n"

const char *ffaacsucks_version() { return FFMPEGAACSUCKS_VERSION; }

bool ffaacsucks_check_aac_stream_packet(AVPacket *pkt, char *filepath) {
  int pkt_type, skip, namelen;

  uint8_t b = pkt->buf->data[0];
  pkt_type = (b & 0xe0) >> 5;
  if (pkt_type != 6) {
    if (getenv("FFMPEGAACSUCKS_DEBUG") != NULL)
      fprintf(
          stderr,
          "%s: unexpected packet type found for Lavc/FFMPEG AAC in stream %d "
          "(%d)\n",
          filepath, pkt->stream_index, pkt_type);
    return false;
  }

  namelen = (b & 0x1e) >> 1;

  if (namelen == 15)
    skip = 3;
  else
    skip = 2;

  char *comment = (char *)pkt->buf->data + skip;
  return (strncmp(comment, FFAACSUCKS_LAVC_SIGNATURE, 4)) == 0;
}

static void ffaacsucks_priv_check_avfcontext(AVFormatContext *s, char *filepath,
                                             struct ffaacsucks_result *res) {
  unsigned int i, aac_streams = 0;
  int ret;

  for (i = 0; i < s->nb_streams; i++) {
    AVStream *st = s->streams[i];

    if (st->codecpar->codec_id == AV_CODEC_ID_AAC) {
      aac_streams++;
    } else {
      st->discard = AVDISCARD_ALL;
    }
  }

  if (aac_streams == 0)
    return;

  res->streams = malloc(sizeof(int) * aac_streams);
  AVPacket *pkt = av_packet_alloc();

  while (aac_streams > 0) {
    ret = av_read_frame(s, pkt);
    if (ret < 0)
      perror("av_read_frame");

    aac_streams--;

    if (ffaacsucks_check_aac_stream_packet(pkt, filepath))
      res->streams[res->n_streams++] = pkt->stream_index;

    s->streams[pkt->stream_index]->discard = AVDISCARD_ALL;
    av_packet_unref(pkt);
  }

  av_packet_free(&pkt);
}

struct ffaacsucks_result *ffaacsucks_check_avfcontext(AVFormatContext *s,
                                                      char *filepath) {
  struct ffaacsucks_result *res = malloc(sizeof(struct ffaacsucks_result));
  res->n_streams = 0;
  res->streams = NULL;

  ffaacsucks_priv_check_avfcontext(s, filepath, res);

  return res;
}

struct ffaacsucks_result *ffaacsucks_check(char *filepath) {
  AVFormatContext *s = NULL;
  int ret;
  struct ffaacsucks_result *res = malloc(sizeof(struct ffaacsucks_result));
  res->n_streams = 0;
  res->streams = NULL;

  ret = avformat_open_input(&s, filepath, NULL, NULL);

  if (ret < 0) {
    perror("failed to load file");
    res->n_streams = -1;
    return res;
  }

  ffaacsucks_priv_check_avfcontext(s, filepath, res);

  avformat_close_input(&s);
  avformat_free_context(s);

  return res;
}

void ffaacsucks_result_free(struct ffaacsucks_result *res) {
  if (res->streams != NULL)
    free(res->streams);
  free(res);
}
