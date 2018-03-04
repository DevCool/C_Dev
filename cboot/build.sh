#!/bin/sh
gcc -m32 -c -g -Os -march=i686 -ffreestanding -fdata-sections -ffunction-sections -Wl,--gc-sections -Wall -Werror -I. -o boot.o boot.c
ld -melf_i386 -static -Tboot.ld -nostdlib --nmagic -o boot.elf boot.o || { echo "ld: There was an error compiling or linking."; exit 1; }
objcopy -O binary boot.elf boot.bin || { echo "objcopy: There was an error copying."; exit 1; }
gcc -m32 -c -g -Os -march=i686 -ffreestanding -fdata-sections -ffunction-sections -Wl,--gc-sections -Wall -Werror -I. -o stage2.o stage2.c
ld -melf_i386 -static -Tstage2.ld -nostdlib --nmagic -o stage2.elf stage2.o || { echo "ld: There was an error compiling or linking."; exit 1; }
objcopy -O binary stage2.elf stage2.bin || { echo "objcopy: There was an error copying."; exit 1; }
