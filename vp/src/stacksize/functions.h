#ifndef RISCV_VP_FUNCTIONS_H
#define RISCV_VP_FUNCTIONS_H

#include <stdint.h>
#include <stddef.h>

#include <unordered_map>

class FuncInfo {
public:
	std::string name;
	size_t      stack_size;

	FuncInfo(std::string _name, size_t _stack_size)
		: name(_name), stack_size(_stack_size) {}
};

class FunctionSet {
private:
	typedef uint64_t Address;

	std::unordered_map<Address, FuncInfo> funcs;
public:
	FunctionSet(std::string filename);
};

#endif
