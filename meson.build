project('ffmpegaacsucks', 'c', version: '1.0.3')

libavdeps = []
libavdeps += dependency('libavformat')
libavdeps += dependency('libavutil')
libavdeps += dependency('libavcodec')

version_h = vcs_tag(command: ['git', 'describe', '--dirty=+', '--always'],
                    input: 'version.h.in', output:'version.h')

install_headers('ffmpegaacsucks.h')

libffmpegaacsucks = library(
  'ffmpegaacsucks',
  'libffmpegaacsucks.c',
  version_h,
  dependencies: libavdeps,
  install: true,
  version: meson.project_version(),
  soversion: '1',
)

ffmpegaacsucks_exe = executable(
  'ffmpegaacsucks',
  'ffmpegaacsucks.c',
  link_with: libffmpegaacsucks,
  install: true,
)

pkg = import('pkgconfig')
pkg.generate(libffmpegaacsucks)

ffmpegaacsucks_dep = declare_dependency(link_with: libffmpegaacsucks)
