#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libffmpegaacsucks.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: ffmpegaacsucks <file>");
        return EXIT_FAILURE;
    }

    if (ffaacsucks_check(argv[1]) == FFMPEGAACSUCKS_DETECTED)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
