ASM=nasm
CC=gcc
BOOTLOADER_FILE=bootloader.s
INIT_KERNEL_FILES=starter.s
KERNEL_FILES=kernel.c
KERNEL_FLAGS = -Wall -m32 -ffreestanding -fno-asynchronous-unwind-tables -fno-pie -fno-stack-protector -c


build: $(BOOTLOADER_FILE) $(KERNEL_FILES)  $(INIT_KERNEL_FILES)
	$(ASM) -f bin $(BOOTLOADER_FILE) -o bootloader.o
	$(ASM) -f elf32 $(INIT_KERNEL_FILES) -o starter.o
	$(CC) $(KERNEL_FLAGS) $(KERNEL_FILES) -o kernel.o
	ld -melf_i386 -Tlinker.ld starter.o kernel.o -o kernel.elf
	objcopy -O binary kernel.elf kernel.bin
	dd if=bootloader.o of=kernel.img
	dd seek=1 conv=sync if=kernel.bin of=kernel.img bs=512 count=5
	dd seek=6 conv=sync if=/dev/zero of=kernel.img bs=512 count=2046

run: clean build
	qemu-system-x86_64 -s kernel.img

debug: clean build
	qemu-system-x86_64 -s -S kernel.img


clean:
	rm -f *.bin *.o kernel.elf kernel.img
