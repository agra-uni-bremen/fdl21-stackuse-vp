override CC = riscv32-unknown-elf-gcc

CFLAGS  = -O0 -ggdb -fstack-usage -march=rv32imc -mabi=ilp32
ASFLAGS = $(CFLAGS)
LDFLAGS = -nostartfiles -Wl,--no-relax

all: main stack_usage

main: main.o bootstrap.o
stack_usage: main
	stack-usage-db $< convert.sh > $@

sim: main stack_usage
	tiny32-vp --stack-usage stack_usage --intercept-syscalls $<

.PHONY: all sim
