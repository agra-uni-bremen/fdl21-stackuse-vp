#ifndef RISCV_VP_STACK_USAGE_H
#define RISCV_VP_STACK_USAGE_H

#include <stdint.h>

#include <fstream>
#include <unordered_map>

#define FSTACK_STATIC  0100
#define FSTACK_DYNAMIC 0200
#define FSTACK_BOUNDED 0300

class FuncStackUsage {
public:
	std::string name;
	size_t      stack_usage;
	uint8_t     types;
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

class StackUsage {
private:
	std::unordered_map<std::string, size_t> funcs;

public:
	StackUsage(std::string name, std::ifstream &stream);
	size_t get_usage(std::string func_name);
};

#endif
