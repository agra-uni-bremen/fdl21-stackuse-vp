override CC = riscv32-unknown-elf-gcc

CFLAGS  = -O -fstack-usage -march=rv32imc -mabi=ilp32
ASFLAGS = $(CFLAGS)
LDFLAGS = -nostartfiles -Wl,--no-relax

all: main
main: main.o bootstrap.o
sim: main
	tiny32-vp --stack-usage main.su --intercept-syscalls $<

.PHONY: all sim
