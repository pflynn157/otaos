CFLAGS=-I./include

OBJS=build/boot2.o \
	build/kernel/kernel.o \
	build/kernel/gdt.o \
	build/kernel/io.o \
	build/kernel/pic.o \
	build/kernel/idt.o \
	build/kernel/idt2.o \
	build/kernel/string.o \
	build/kernel/stdio.o \
	build/kernel/keyboard.o \
	build/kernel/software.o \
	build/kernel/ata.o \
	build/kernel/tty.o

all: check build/os.img

.PHONY: check
check:
	if [ ! -d ./build ]; then mkdir -p build; fi
	if [ ! -d ./build/kernel ] ; then mkdir -p build/kernel; fi
    
build/os.img: build/os.bin
	dd if=/dev/zero of=$@ bs=1024 count=1024 conv=notrunc
	dd if=$^ of=$@ bs=512 conv=notrunc
	
build/os.bin: build/boot.bin build/kernel.elf
	cat build/boot.bin build/kernel.elf > $@
	
build/boot.bin: boot/boot1.asm
	nasm -f bin $^ -o $@
	
build/kernel.elf: $(OBJS)
	i686-elf-ld -o $@ -Ttext=0x1000 $^ --oformat=binary
	
build/boot2.o: boot/boot2.asm
	nasm -f elf32 $^ -o $@
	
build/kernel/%.o: kernel/%.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@
	
build/kernel/%.o: kernel/%.asm
	nasm -f elf32 $< -o $@
	
.PHONY: clean
clean:
	rm build/*.o; \
	rm build/*.bin; \
	rm build/*.img; \
	rm build/*.elf; \
	rm build/kernel/*;
