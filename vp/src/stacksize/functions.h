#ifndef RISCV_VP_STACKSIZE_H
#define RISCV_VP_STACKSIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <unordered_map>

#include "stack_usage.h"

class FuncInfo {
public:
	std::string name;
	size_t stack_size;

	FuncInfo(std::string _name, size_t _stack_size)
		: name(_name), stack_size(_stack_size) {}
};

class FunctionSet {
public:
	typedef uint64_t Address;

	FunctionSet(std::string elf, std::string acc_stack_usage);

	bool has_func(Address addr);
	FuncInfo &get_func(Address addr);

private:
	StackUsage stack;
	std::unordered_map<Address, FuncInfo> funcs;
};

#endif
