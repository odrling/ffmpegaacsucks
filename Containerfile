FROM alpine AS builder 

COPY . /ffmpegaacsucks

RUN apk add musl-dev meson clang compiler-rt git linux-headers lld tar
RUN cd /ffmpegaacsucks && /ffmpegaacsucks/ci/build.sh

FROM alpine

COPY --from=builder /ffmpegaacsucks/dest/ /
ENTRYPOINT ["/usr/local/bin/ffmpegaacsucks"]

