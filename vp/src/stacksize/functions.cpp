#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <elfutils/libdwfl.h>

#include <iostream>
#include <system_error>

#include "functions.h"

static char *debuginfo_path;
static const Dwfl_Callbacks offline_callbacks = (Dwfl_Callbacks){
	.find_elf = dwfl_build_id_find_elf,
	.find_debuginfo = dwfl_standard_find_debuginfo,

	.section_address = dwfl_offline_section_address,
	.debuginfo_path = &debuginfo_path,
};

FunctionSet::FunctionSet(std::string elf, std::string acc_stack_usage)
  : stack(acc_stack_usage)
{
	int n, fd;
	Dwfl_Module *mod;
	Dwfl *dwfl = nullptr;
	const char *elffn = elf.c_str();

	if ((fd = open(elffn, O_RDONLY)) == -1)
		throw std::system_error(errno, std::generic_category());
	if (!(dwfl = dwfl_begin(&offline_callbacks)))
		goto err;

	dwfl_report_begin(dwfl);
	if (!(mod = dwfl_report_offline(dwfl, elffn, elffn, fd)))
		goto err;
	if ((n = dwfl_module_getsymtab(mod)) == -1)
		goto err;

	for (int i = 0; i < n; i++) {
		GElf_Sym sym;
		GElf_Addr addr;
		const char *name;
		size_t stacksize;

		name = dwfl_module_getsym_info(mod, i, &sym, &addr, NULL, NULL, NULL);
		if (!name || GELF_ST_TYPE(sym.st_info) != STT_FUNC)
			continue;

		try {
			stacksize = stack.get_usage(name);
		} catch (const std::out_of_range&) {
			std::cerr << "couldn't determine stack usage for: " + std::string(name) << std::endl;
			continue;
		}

		funcs.insert(std::make_pair<Address, FuncInfo>(
			(Address)addr,
			FuncInfo(std::string(name), stacksize)
		));
	}

	dwfl_report_end(dwfl, NULL, NULL);
	return;
err:
	if (dwfl)
		dwfl_end(dwfl);
	close(fd);

	throw std::system_error(EIO, std::generic_category());
}

bool
FunctionSet::has_func(Address addr)
{
	return funcs.count(addr) > 0;
}

FuncInfo &
FunctionSet::get_func(Address addr)
{
	return funcs.at(addr);
}
