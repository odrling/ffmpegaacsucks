ARG ARCH
FROM ghcr.io/odrling/gentoo-crossdev-images:$ARCH-llvm17 AS builder 

COPY . /ffmpegaacsucks

ARG ARCH

RUN cd /ffmpegaacsucks && /ffmpegaacsucks/ci/build.sh --cross-file ci/$ARCH-unknown-linux-musl.txt

FROM alpine

COPY --from=builder /ffmpegaacsucks/dest/ /
ENTRYPOINT ["/usr/local/bin/ffmpegaacsucks"]

