project(
    'ffmpegaacsucks',
    'c',
    license: 'LGPL-3.0-only',
    version: '2.0.1',
    default_options: ['warning_level=3', 'c_std=gnu17'],
)

libavdeps = []
libavdeps += dependency('libavformat', fallback: ['ffmpeg', 'libavformat_dep'])
libavdeps += dependency('libavutil', fallback: ['ffmpeg', 'libavutil_dep'])
libavdeps += dependency('libavcodec', fallback: ['ffmpeg', 'libavcodec_dep'])

git = find_program('git', required: false)

if git.found()
    # avoids detection issues in some cases, such as git worktrees
    version_h = vcs_tag(
        command: [git, 'describe', '--dirty=+', '--always'],
        input: 'version.h.in',
        output: 'version.h',
    )
else
    version_h = vcs_tag(input: 'version.h.in', output: 'version.h')
endif

install_headers('include/ffmpegaacsucks.h')
inc = include_directories('include')

libffmpegaacsucks = library(
    'ffmpegaacsucks',
    'libffmpegaacsucks.c',
    version_h,
    include_directories: inc,
    dependencies: libavdeps,
    install: true,
    version: meson.project_version(),
    soversion: '3',
)

ffmpegaacsucks_exe = executable(
    'ffmpegaacsucks',
    'ffmpegaacsucks.c',
    include_directories: inc,
    dependencies: libavdeps,
    link_with: libffmpegaacsucks,
    install: true,
)

pkg = import('pkgconfig')
pkg.generate(libffmpegaacsucks)

ffmpegaacsucks_dep = declare_dependency(
    link_with: libffmpegaacsucks,
    dependencies: libavdeps,
    include_directories: [inc],
)
