# Declare constants for the multiboot header.
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002
.set CHECKSUM,  -(MAGIC + FLAGS)

# Declare a header as in the Multiboot Standart.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

# The kernel entry point.
.section .text 
.global _start
.type start, @function
_start:
    movl $stack_top, %esp

    # Call the global constructors
    call _init 

    # Transfer control to the main kernel.
    call kernel_main

    # Hand if kernel_main unexpectedly returns 
    cli
1:  hlt
    jmp 1b
.size _start, . - _start
