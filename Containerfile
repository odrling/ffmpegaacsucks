FROM ghcr.io/odrling/chimera-images:main AS builder
ARG ARCH

RUN apk add musl-devel meson clang git lld

COPY . /ffmpegaacsucks

RUN cd /ffmpegaacsucks && ARCH=$ARCH ci/build.sh

FROM alpine

COPY --from=builder /ffmpegaacsucks/dest/ /
ENTRYPOINT ["/usr/local/bin/ffmpegaacsucks"]

