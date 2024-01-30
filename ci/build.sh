#!/bin/sh -xe
export LDFLAGS="-Wl,-Bsymbolic"

if [ "$ARCH" != x86_64 ]; then
    apk add "base-cross-$ARCH" "clang-rt-cross-$ARCH" "clang-rt-crt-cross-$ARCH" 
    extra_args="--cross-file ci/$ARCH-chimera-linux-musl.txt"
fi

meson setup build --buildtype release --strip -Dffmpeg:programs=disabled -Dffmpeg:tests=disabled -Dffmpeg:encoders=disabled -Dffmpeg:muxers=disabled -Dffmpeg:avfilter=disabled -Dffmpeg:avdevice=disabled -Dffmpeg:postproc=disabled -Dffmpeg:swresample=disabled -Dffmpeg:swscale=disabled -Dffmpeg:decoders=disabled -Dffmpeg:aac_decoder=enabled -Dffmpeg:aac_fixed_decoder=enabled -Dffmpeg:aac_latm_decoder=enabled -Dffmpeg:version3=enabled $extra_args
cat /ffmpegaacsucks/build/meson-logs/meson-log.txt

meson compile -C build
meson install -C build --destdir=../dest/
