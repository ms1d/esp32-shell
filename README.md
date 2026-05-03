# esp32-shell

A small mock up shell for the esp32 s3 microcontroller, using esp-idf

## Features

- Interactive Shell prompt display for a SH1106 128x64 display

- Easy to hard-code commands which simply return a text output

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

- `idf.py -p /dev/tty<target_port> build flash` to build and flash the MCU
