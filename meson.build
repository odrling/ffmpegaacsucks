project('ffmpegaacsucks', 'c')

libavdeps = []
libavdeps += dependency('libavformat')
libavdeps += dependency('libavutil')
libavdeps += dependency('libavcodec')

ffmpegaacsucks_exe = executable(
  'ffmpegaacsucks',
  'ffmpegaacsucks.c',
  dependencies: libavdeps,
  install: true,
)
