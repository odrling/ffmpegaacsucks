#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
#include <libavcodec/codec_id.h>
#include <libavutil/dict.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int ffaacsucks_check(char* filepath) {
    AVFormatContext *s = NULL;
    unsigned int i, aac_streams;
    int ret, return_value = EXIT_SUCCESS;

    ret = avformat_open_input(&s, filepath, NULL, NULL);

    if (ret < 0) {
        perror("failed to load file");
        exit(EXIT_FAILURE);
    }
    aac_streams = 0;

    for (i = 0; i < s->nb_streams; i++) {
        AVStream *st = s->streams[i];

        if (st->codecpar->codec_id == AV_CODEC_ID_AAC) {
            // first aac stream id
            aac_streams++;
        } else {
            st->discard = AVDISCARD_ALL;
        }
    }

    #define NOT_FOUND_STR "no Lavc/FFmpeg AAC stream was found\n"

    if (aac_streams == 0) {
        printf(NOT_FOUND_STR);
        return_value = EXIT_FAILURE;
        goto free_avf;
    }

    AVPacket *pkt = av_packet_alloc();

    while (aac_streams > 0) {
        ret = av_read_frame(s, pkt);
        if (ret < 0)
            perror("av_read_frame");

        // skip first 3 bytes 
        char *comment = (char *) pkt->buf->data + 3;
        int cmp = strncmp(comment, "Lavc", 4);
        aac_streams--;
        if (cmp == 0) {
            printf("stream %d is a Lavc/FFmpeg AAC stream\n", pkt->stream_index);
        } else if (aac_streams == 0) {
            printf(NOT_FOUND_STR);
            return_value = EXIT_FAILURE;
        }

        av_packet_unref(pkt);
    }

    av_packet_free(&pkt);

free_avf:
    avformat_close_input(&s);
    avformat_free_context(s);

    return return_value;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return EXIT_FAILURE;

    return ffaacsucks_check(argv[1]);
}
