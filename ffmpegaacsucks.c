#include <stdio.h>
#include <stdlib.h>

#include "libffmpegaacsucks.h"

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(stderr, "usage: ffmpegaacsucks <file>");
        return EXIT_FAILURE;
    }

    struct ffaacsucks_result *ffa_res = ffaacsucks_check(argv[1]);
    if (ffa_res->n_streams > 0) {
        for (i = 0; i < ffa_res->n_streams; i++)
            printf("stream %d is a Lavc/FFmpeg AAC stream\n", i);

        ffaacsucks_result_free(ffa_res);
        return EXIT_SUCCESS;
    } else {
        if (ffa_res->n_streams < 0)
            fprintf(stderr, "failed to analyse file %s", argv[1]) ;
        else
            printf("no Lavc/FFmpeg AAC stream found.");

        ffaacsucks_result_free(ffa_res);
        return EXIT_FAILURE;
    }
}
