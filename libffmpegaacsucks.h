#ifndef FFMPEGAACSUCKS_H
#define FFMPEGAACSUCKS_H 1

#define FFMPEGAACSUCKS_DETECTED 1
#define FFMPEGAACSUCKS_UNDETECTED 0
#define FFMPEGAACSUCKS_FAILURE -1

struct ffaacsucks_result {
    int n_streams;
    int *streams;
};

struct ffaacsucks_result* ffaacsucks_check(char* filepath);

void ffaacsucks_result_free(struct ffaacsucks_result* res);

#endif
