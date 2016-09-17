cmake_minimum_required(VERSION 3.5)
project(teensy)

include_directories("/home/jabelone/uni/CAB202/teensy/cab202_teensy")
SET(CMAKE_C_COMPILER /usr/bin/avr-gcc-4.9.2)
SET(CMAKE_CXX_COMPILER /usr/bin/avr-g++)

SET(CMAKE_C_FLAGS "-mmcu=atmega32u4 -Os -DF_CPU=8000000UL -std=gnu99 -Wall -Werror")
SET(CMAKE_C_LINK_FLAGS "-mmcu=${MCU}")

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(SOURCE_FILES
    cab202_teensy/ascii_font.h
    cab202_teensy/cpu_speed.h
    cab202_teensy/graphics.c
    cab202_teensy/graphics.h
    cab202_teensy/graphics.lst
    cab202_teensy/graphics.o
    cab202_teensy/lcd.c
    cab202_teensy/lcd.h
    cab202_teensy/lcd.lst
    cab202_teensy/lcd.o
    cab202_teensy/libcab202_teensy.a
    cab202_teensy/macros.h
    cab202_teensy/Makefile
    cab202_teensy/ram_utils.c
    cab202_teensy/ram_utils.h
    cab202_teensy/ram_utils.lst
    cab202_teensy/ram_utils.o
    cab202_teensy/sprite.c
    cab202_teensy/sprite.h
    cab202_teensy/sprite.lst
    cab202_teensy/sprite.o
    49-teensy.rules
    lol.c
    lol.elf
    lol.hex
    lol.o
    Makefile
    QUTTeensy_test.hex
    teensy.64bit)

add_executable(teensy ${SOURCE_FILES})