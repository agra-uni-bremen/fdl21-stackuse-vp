#ifndef RISCV_VP_ELF_FILE_H
#define RISCV_VP_ELF_FILE_H

#include <stdint.h>

#include <map>
#include <string>
#include <memory>
#include <vector>

#include <elfutils/libdwfl.h>

class ELFFile {
	int fd = -1;
	Dwfl *dwfl = nullptr;
	Dwfl_Module *mod = nullptr;

	std::map<std::string, uint64_t> symbols;
	void init(void);

public:
	ELFFile(std::string path);
	~ELFFile(void);

	uint64_t get_symbol(std::string name);
};

#endif
