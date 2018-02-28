#!/bin/sh
echo "Starting to make image..."
dd if=/dev/zero of=floppy.img bs=1024 count=1440
if [ ! "$?" = "0" ]; then
	echo "There was an error making the floppy image."
	exit 1
fi
dd if=vbr.bin of=floppy.img bs=512 count=1 conv=notrunc
if [ ! "$?" = "0" ]; then
	echo "There was an error copying to floppy image."
	exit 1
fi
echo "Done."
