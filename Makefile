
CROSS_PREFIX = i686-elf
CC = $(CROSS_PREFIX)-gcc
AS = nasm
LD = $(CROSS_PREFIX)-ld


CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./include
ASFLAGS = -f elf32
LDFLAGS = -T scripts/linker.ld -nostdlib

BOOT_DIR = boot
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
BUILD_DIR = build


BOOT_SRC = $(wildcard $(BOOT_DIR)/*.asm)
KERNEL_SRC = $(wildcard $(KERNEL_DIR)/*.c)
DRIVERS_SRC = $(wildcard $(DRIVERS_DIR)/*.c)

BOOT_OBJ = $(BOOT_SRC:.asm=.o)
KERNEL_OBJ = $(KERNEL_SRC:.c=.o)
DRIVERS_OBJ = $(DRIVERS_SRC:.c=.o)


KERNEL_BIN = $(BUILD_DIR)/kernel.bin

.PHONY: all clean run

all: $(BUILD_DIR) $(KERNEL_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(DRIVERS_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

run: $(KERNEL_BIN)
	qemu-system-i386 -kernel $(KERNEL_BIN)

clean:
	rm -rf $(BUILD_DIR) $(BOOT_OBJ) $(KERNEL_OBJ) $(DRIVERS_OBJ) 