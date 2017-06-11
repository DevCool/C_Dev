ASFLAGS := -32
CFLAGS := -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -lgcc
LDFLAGS := -T src/linker.ld -melf_i386

all: kernel

kernel: make_bin compile_entry compile_kernel link_kernel

make_bin:
	mkdir -p bin

compile_entry: src/entry.s
	$(AS) $(ASFLAGS) $^ -o bin/entry.o

compile_kernel: src/kernel.c
	$(CC) $(CFLAGS) -c $^ -o bin/kernel.o

link_kernel:
	$(LD) $(LDFLAGS) -o bin/kernel.bin bin/entry.o bin/kernel.o

full-iso: clean kernel image

run: full-iso run-iso

run-iso:
	@echo "Running image."
	qemu-system-i386 -cdrom tinyos.iso
	@echo "Done running."

image:
	@echo "Making tinyos iso..."
	mkdir -p iso/boot/grub
	cp bin/kernel.bin iso/boot/kernel.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o tinyos.iso iso
	@echo "Done with iso."

clean_image:
	rm -rf iso tinyos.iso

clean:
	@echo "Cleaning up..."
	rm -rf bin *~ **/*~ iso *.iso
	@echo "Cleaning done."

