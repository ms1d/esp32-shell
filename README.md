# esp32-shell

A small mock up shell for the esp32 s3 microcontroller, using esp-idf

## Features

- Interactive Shell prompt display for a SH1106 128x64 display

- Easy to hard-code commands which simply return a text output

- `help` returns the name of a random command

## Layout

- Source code under `main/`

- Main program in `main.c`

- U8G2 boilerplate (for OLED screen) in `u8g2.h`

- Keypad driver in `keypad.h`

- Key pin numbers in `pins.h` for easy wire relocation

- Definitions in header files to reduce project complexity

## Setup

- Clone repo (`git clone https://github.com/ms1d/esp32-shell.git`)

- `git submodule addhttps://github.com/olikraus/u8g2.git` to add u8g2 (under components)

- Install `esp-idf` & `set-target esp32s3`

- Wire up MCU to a `SPI SH1106 OLED screen` & `4x4 Matrix Keypad`

- Create `main/data.h` locally. It should contain:

  - `const char *cmd_names[]`

  - `const char *cmd_outputs[]`

  - `const int cmd_count`

  - `const int cmd_name_lens[]`

  - `const int cmd_lens[]`

- `idf.py -p /dev/tty<target_port> build flash` to build and flash the MCU

## Notes & Quirks

- `cmds[i]` must be longer than the length of the shell PROMPT to overwrite it

- `cmds[i]` must be shorter than BUFFER_SIZE; otherwise it will crash the program

- `help` is reserved by the program and is ignored if you include it

## Sample `main/data.h`

``` C
#define ARR_LEN(a) (sizeof(a) / sizeof(a[0]))
#define STR_LEN(s) (sizeof(s) - 1)

const char *cmd_names[] = {
  "hello world"
};

const int cmd_count = ARR_LEN(cmd_names);

const char *cmds[] = {
  "hello world!!!!!!"
};

_Static_assert(ARR_LEN(cmd_names) == ARR_LEN(cmds),
"cmd_names and cmds must be the same length");

#include <string.h>

int cmd_name_lens[ARR_LEN(cmd_names)];
int cmd_lens[ARR_LEN(cmds)];

// Run at startup (main/main.c's app_main())
void init_data() {
    for (size_t i = 0; i < ARR_LEN(cmd_names); i++) {
        cmd_name_lens[i] = strlen(cmd_names[i]);
        cmd_lens[i]      = strlen(cmds[i]);
    }
}
```
