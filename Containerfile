FROM ghcr.io/odrling/chimera:x86_64 AS builder
ARG ARCH

RUN apk upgrade && apk add musl-devel meson clang git lld

COPY . /ffmpegaacsucks

RUN cd /ffmpegaacsucks && ARCH=$ARCH ci/build.sh

FROM ghcr.io/odrling/chimera

COPY --from=builder /ffmpegaacsucks/dest/ /
ENTRYPOINT ["/usr/local/bin/ffmpegaacsucks"]

