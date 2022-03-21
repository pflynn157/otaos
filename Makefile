CFLAGS=-I./include

KERNEL_SRC=$(wildcard kernel/*.c)
KERNEL_OBJS=$(addprefix build/,$(patsubst %.c, %.o, $(KERNEL_SRC)))
KERNEL_ASM_SRC=$(wildcard kernel/*.asm)
KERNEL_ASM_OBJS=$(addprefix build/,$(patsubst %.asm, %.o, $(KERNEL_ASM_SRC)))

BOOT_OBJS=build/boot2.o

OBJS=$(BOOT_OBJS) \
	$(KERNEL_OBJS) \
	$(KERNEL_ASM_OBJS)

all: check build/os.img

.PHONY: check
check:
	if [ ! -d ./build ]; then mkdir -p build; fi
	if [ ! -d ./build/kernel ] ; then mkdir -p build/kernel; fi
    
build/os.img: build/os.bin
	dd if=/dev/zero of=$@ bs=1024 count=1024 conv=notrunc
	dd if=$^ of=$@ bs=512 conv=notrunc
	
build/os.bin: build/boot.bin build/kernel.elf build/fs.img
	cat build/boot.bin build/kernel.elf build/fs.img > $@
	
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

.PHONY: build/fs.img	
build/fs.img:
	nasm -f bin odisk/disk.asm -o build/fs.img
	
#build/odisk: odisk/odisk.c
#	$(CC) odisk/odisk.c -o build/odisk
	
.PHONY: clean
clean:
	rm build/*.o; \
	rm build/*.bin; \
	rm build/*.img; \
	rm build/*.elf; \
	rm build/kernel/*;
	
