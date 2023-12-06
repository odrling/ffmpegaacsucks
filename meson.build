project('ffmpegaacsucks', 'c', version: '1.0.0')

libavdeps = []
libavdeps += dependency('libavformat')
libavdeps += dependency('libavutil')
libavdeps += dependency('libavcodec')

version_h = vcs_tag(input: 'version.h.in', output:'version.h')

install_headers('ffmpegaacsucks.h')

libffmpegaacsucks = library(
  'ffmpegaacsucks',
  'libffmpegaacsucks.c',
  version_h,
  dependencies: libavdeps,
  install: true,
)

ffmpegaacsucks_exe = executable(
  'ffmpegaacsucks',
  'ffmpegaacsucks.c',
  link_with: libffmpegaacsucks,
  install: true,
)
