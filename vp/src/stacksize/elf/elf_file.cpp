#include "elf_file.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <system_error>

/* Taken from elfutils example code. */
static char *debuginfo_path;
static const Dwfl_Callbacks offline_callbacks = (Dwfl_Callbacks){
	.find_elf = dwfl_build_id_find_elf,
	.find_debuginfo = dwfl_standard_find_debuginfo,

	.section_address = dwfl_offline_section_address,
	.debuginfo_path = &debuginfo_path,
};

ELFFile::ELFFile(std::string path) {
	const char *fn = path.c_str();

	if ((fd = open(fn, O_RDONLY)) == -1)
		throw std::system_error(errno, std::generic_category());
	if (!(dwfl = dwfl_begin(&offline_callbacks)))
		throw std::runtime_error("dwfl_begin failed");
}

ELFFile::~ELFFile(void) {
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}

	if (dwfl) {
		dwfl_end(dwfl);
		dwfl = nullptr;
	}
}

void
ELFFile::init(void) {
	int n;
	Dwfl_Module *mod;

	if (!(mod = dwfl_report_offline(dwfl, "main", "main", fd)))
		throw std::runtime_error("dwfl_report_offline failed");
	if ((n = dwfl_module_getsymtab(mod)) == -1)
		throw std::runtime_error("dwfl_module_getsymtab failed");

	for (int i = 0; i < n; i++) {
		GElf_Sym sym;
		GElf_Addr addr;
		const char *name;

		name = dwfl_module_getsym_info(mod, i, &sym, &addr, NULL, NULL, NULL);
		if (!name || *name == '\0')
			continue;

		(void)sym; // TODO

		// XXX: names are not unique.
		symbols[name] = (uint64_t)addr;
	}

	dwfl_report_end(dwfl, NULL, NULL);
}

uint64_t
ELFFile::get_symbol(std::string name) {
	return symbols.at(name);
}
