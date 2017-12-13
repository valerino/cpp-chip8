# VUE core launcher

This is the *VUE* launcher, will launch a core with the specified rom image to be run.

## directory layout
*emulation cores* must be copied in *emulators/core_name* in the same directory as *core* binary.

```
[valerino@agilulfo] ls
core      core.json emulators

[valerino@agilulfo] ls -l emulators
total 0
drwxr-xr-x  6 valerino  staff  192 Dec 13 02:51 chip8

[valerino@agilulfo] ls -l emulators/chip8
total 952
-rw-r--r--   1 valerino  staff    1332 Dec 13 02:34 chip8.json
-rwxr-xr-x   1 valerino  staff  479064 Dec 13 02:51 libchip8.dylib
drwxr-xr-x@ 64 valerino  staff    2048 Nov 21 01:18 roms
```

## usage
```
I] VUE - valerino's universal emulator
(c)opyleft, 2k17

[I] 	usage: ./core -e <emu_core> -o [core_options] -r <image>
	usage: ./core -l to list available cores
```

* list emulation cores
```
[valerino@agilulfo] ./core -l
[I] VUE - valerino's universal emulator
(c)opyleft, 2k17

[I] available cores:
	core: vue-chip8
		info: Chip8/Super-Chip8 emulator
		version: v1.0
```

* run image
```
[valerino@agilulfo] ./core -e chip8 -r emulators/chip8/roms/super-chip8/SuperWorm\ V3\ \[RB,\ 1992\].ch8
[I] loading emulator core: chip8
[D] NOTIFY: SDL initialized
[D] NOTIFY: loaded: emulators/chip8/roms/super-chip8/SuperWorm V3 [RB, 1992].ch8
```