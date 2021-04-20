#ifndef RISCV_VP_FUNCTIONS_H
#define RISCV_VP_FUNCTIONS_H

#include <stdint.h>
#include <stddef.h>

#include <vector>

class FuncInfo {
public:
	std::string name;
	uint64_t    addr;
	size_t      stack_size;

	FuncInfo(std::string _name, uint64_t _addr, size_t _stack_size)
		: name(_name), addr(_addr), stack_size(_stack_size) {}
};

class FunctionSet {
private:
	std::vector<FuncInfo> funcs;
public:
	FunctionSet(std::string filename);
};

#endif
