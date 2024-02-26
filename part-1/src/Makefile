kernel.img: bootloader.bin kernel.bin
	dd if=bootloader.bin of=kernel.img
	dd bs=512  seek=1 conv=sync if=kernel.bin of=kernel.img

	qemu-system-x86_64 -s kernel.img

bootloader.bin: bootloader.s
	nasm -f bin bootloader.s -o bootloader.bin

kernel.bin: kernel.s
	nasm -f bin kernel.s -o kernel.bin

clean:
	rm -f *.bin kernel.img
