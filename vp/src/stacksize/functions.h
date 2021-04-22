#ifndef RISCV_VP_STACKSIZE_H
#define RISCV_VP_STACKSIZE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <unordered_map>

class FuncInfo {
public:
	std::string name;
	uint64_t addr;
	size_t stack_size;

	FuncInfo(void) : name(""), addr(0), stack_size(0) {}

	FuncInfo(std::string _name, uint64_t _addr, size_t _stack_size)
		: name(_name), addr(_addr), stack_size(_stack_size) {}
};

class ParserError : public std::exception {
	std::string fileName, msg, whatstr;
	size_t line;

	public:
	ParserError(std::string _fileName, size_t _line, std::string _msg)
		: fileName(_fileName), msg(_msg), line(_line)
	{
		this->whatstr = fileName + ":" + std::to_string(line) + ": " + msg;
	}

	const char *what(void) const throw()
	{
		return whatstr.c_str();
	}
};

class FunctionSet {
public:
	typedef uint64_t Address;

	FunctionSet(std::string fp);

	bool has_func(Address addr);
	FuncInfo &get_func(Address addr);

private:
	std::unordered_map<Address, FuncInfo> funcs;
};

#endif
