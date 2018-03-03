#!/bin/sh
. ./build.sh && . ./makedisk.sh || exit 1
which qemu-system-i386 1> /dev/null
if [ ! "$?" = "0" ]; then
	echo "qemu-system-i386 is not installed."
	exit 1
fi
qemu-system-i386 -fda floppy.img -boot a
