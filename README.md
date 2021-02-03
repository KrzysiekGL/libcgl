# CGL - Chris OpenGL engine
---
Current state of this library is heavily WIP, but it works. For me at least.

## Required Libraries
All of used libraries to compile CGL and/or application using CGL are:
```bash
./include/
├── assimp
├── CGL # for an application only
├── GL
├── GLFW
├── glm
└── SOIL2

./lib/
├── libassimp.so
├── libCGL.a # for an application only
├── libGLEW.a
├── libglfw3.a
└── libsoil2.a
```
And few more installed with linux package manager:
- GL # if on windows, this is the only one required from these
- dl
- X11
- pthread

## Build
To build this library, go under Debug dir, and do the following:

```bash
$ make all
```

If you don't want to build it yourself, you can download already build version
of libCGL.a from release.
(Currently build only for LinuxGCC).

You will get then a `libCGL.a` static library file which you can use in your
project. In the `include` are stored symlinks to all `src/*.h` header files.

But using this library might be quite difficult as of now, because there is no
documentation. (Yes, there might be some day)
