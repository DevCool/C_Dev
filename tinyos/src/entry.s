.set ALIGN,    1<<0		#align loaded modules on page bounds
.set MEMINFO,  1<<1		#private memory map
.set FLAGS,    ALIGN | MEMINFO	# this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002	# 'magic number' lets bootloader find header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp
	call p_main

	cli
	hlt
.Lhang:
	jmp .Lhang

# Set the size of the _start symbol to the current location '.' minus its start
# Needed for debugging or implementing call tracing.
.size _start, . - _start
