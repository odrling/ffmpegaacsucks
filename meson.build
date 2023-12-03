project('ffmpegaacsucks', 'c')

libavdeps = []
libavdeps += dependency('libavformat')
libavdeps += dependency('libavutil')
libavdeps += dependency('libavcodec')

install_headers('ffmpegaacsucks.h')

libffmpegaacsucks = library(
  'ffmpegaacsucks',
  'libffmpegaacsucks.c',
  dependencies: libavdeps,
  install: true,
)

ffmpegaacsucks_exe = executable(
  'ffmpegaacsucks',
  'ffmpegaacsucks.c',
  link_with: libffmpegaacsucks,
  install: true,
)
