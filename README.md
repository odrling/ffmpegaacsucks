# ffmpegaacsucks

```
ffmpegaacsucks <FILE|URL>
```

return code 0 means that the file contains a Lavc/FFmpeg stream.

## How it works

The AAC encoder from libavcodec adds his own header to any stream that is
encoded "Lavc" followed by the version number.
This program reads the encoded stream to check if this header is present.

## Compiling

```
meson setup build
meson compile -C build
```

You should find an executable called `ffmpegaacsucks` in the `build/` directory.
