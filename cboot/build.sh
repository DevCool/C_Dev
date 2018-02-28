#!/bin/sh
gcc -m32 -c -g -Os -march=i686 -ffreestanding -Wall -Werror -I. -o vbr.o vbr.c
ld -melf_i386 -static -Ttest.ld -nostdlib --nmagic -o vbr.elf vbr.o
objcopy -O binary vbr.elf vbr.bin
