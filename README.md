# VUE - valerino's universal emulator
Yet another multi-emulator!

# motivations
This is my attempt to an universal emulator (a-la-MAME, MESS, etc...).
I always been interested in emulation, and the time has come i attempt my
personal take on it :)

> this is a very WIP project!
> the only emulator core available for now is the *chip-8* core, anyway it
> is probably the most accurate chip-8 emulator out there, supporting every
> documented and undocumented chip8 stuff :)

# prerequisites
sdl2, cmake, clang (c++11).

Should run everywhere SDL is ported on, tested on OSX.

# build
This will build *core launcher* and all the available cores.

```
mkdir build
cd build
cmake ..
make
```

# usage
please refer to *src/core/README.md* and to available cores individual *README.md* 
in the *src/emulators* folder.

# TODO
* next core will be Nintendo Gameboy or Nintendo NES.
