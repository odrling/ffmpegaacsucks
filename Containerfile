ARG BUILDER_IMAGE=ghcr.io/odrling/chimera:cross
FROM ${BUILDER_IMAGE} AS builder
ARG ARCH

COPY . /ffmpegaacsucks

RUN cd /ffmpegaacsucks && ARCH=$ARCH ci/build.sh

FROM ghcr.io/odrling/chimera

COPY --from=builder /ffmpegaacsucks/dest/ /
ENTRYPOINT ["/usr/local/bin/ffmpegaacsucks"]

