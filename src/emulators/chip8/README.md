# CHIP8 emulator
This is a highly accurate Chip8 emulator, made as a demo for my universal emulator :)

https://en.wikipedia.org/wiki/CHIP-8

## Implemented
* Chip-8 mode
* Super Chip-8 mode
* Half-pixel in Chip-8 mode (passes HAP test)
* Interpreter fixes to work-around bugs in existing games (can be toggled on via the *fix* options present in the configuration)
* Sokoban works :)

## Usage
Run *VUE* specifying the *chip8* core, i.e.:

```
./core -e chip8 -r emulators/chip8/roms/Tic-Tac-Toe\ \(19xx\)\(Winter,\ David\)\[b\].bin
```

## Configuration
Here's a sample *chip8.json* configuration to be put in *bin/emulators/chip8* folder.

> if not present, a default configuration will be generated!

```
{
    // verbose debug messages (to find CPU bugs mostly, default is off!)
    "dbg_verbose":true,

    // emulate eti-660 (changes the rom start address), default is false
    "mode_eti660": false,

    // some games breaks if vertical wrap is enabled (default)
    "fix_disable_vertical_wrap":false,

    // some games doesn't work correctly if FX55 and FX65 increments I (default)
    "fix_fx55_fx65":false,

    // some games doesn't work correctly if 8XY6 and 8XYE shifts VY (default)
    "fix_8xy6_8xye":false,
    
    // the cpu instruction fixes are automatically enabled on super-chip8 unless this is specified (default is false)
    "disable_sc8_autofix": false,

    // scale the display (10x is the default)
    "display_scale": 10.0,

    // display full screen (windowed is the default)
    "display_fullscreen": false,

    // draw color ("white", "red", "green", "blue", green is the default)
    "display_draw_color": "white",

    // map chip-8 keypad to keys (the following is the default, for cursor keys use SDL names as 'Up', 'Down', etc...)
    "key_0":"0",
    "key_1":"1",
    "key_2":"2",
    "key_3":"3",
    "key_4":"4",
    "key_5":"5",
    "key_6":"6",
    "key_7":"7",
    "key_8":"8",
    "key_9":"9",
    "key_a":"a",
    "key_b":"b",
    "key_c":"c",
    "key_d":"d",
    "key_e":"e",
    "key_f":"f"
}
```

## Defaults
* *esc* : exit emulator
* *P* : pause emulator
* keypad mapped to 0123456789ABCDEF
* no cpu fixes, vertical wrapping enabled, debug messages disabled, green color

## Addendum
Some (standard chip8) games doesn't work at all or partially with the standard Chip-8 VM interpreter as defined in the COSMAC-VIP specifications from mid-1970s.

This emulator attempts to provide settings to overcome these limitations.

>> the cpu instruction fixes *fix_fx55_fx65* and *fix_8xy6_8xye* are automatically applied for Super-Chip8 games, unless *disable_sc8_autofix* is specified

### Known games which needs fix_disable_vertical_wrap = true
* Mines

### Known games which needs fix_fx55_fx65 = true
* Tic-Tac-Toe
* Syzygy

### Known games which needs  fix_8xy6_8xye = true
* Space Invaders
* V-Brix

## TODO
Should be completed unless bugs are found :)
