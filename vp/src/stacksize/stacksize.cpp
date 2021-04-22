#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include <regex>
#include <fstream>
#include <iostream>
#include <system_error>

#include "stacksize.h"

#define REG "([0-9a-zA-Z][0-9a-zA-Z]*)	(..*)	([0-9][0-9]*)"
#define SUB 4 /* match includes entire string */

static bool
parseAddr(uint64_t *out, std::string input)
{
	int r;

	r = sscanf(input.c_str(), "%" PRIx64 "", out);
	return r == 1;
}

static bool
parseSize(size_t *out, std::string input)
{
	unsigned long long r;

	/* reset errno for error check */
	errno = 0;

	r = strtoull(input.c_str(), NULL, 10);
	if (errno != 0)
		return false;

	assert(r <= SIZE_MAX);
	*out = (size_t)r;

	return true;
}

static bool
parseUsage(FuncInfo &usage, std::string line)
{
	std::regex re(REG);
	std::smatch match;

	if (regex_search(line, match, re)) {
		if (match.size() != SUB)
			return false;

		usage.name = match[2].str();
		if (!parseAddr(&usage.addr, match[1].str()))
			return false;
		if (!parseSize(&usage.stack_size, match[3].str()))
			return false;

		return true;
	}

	return false;
}

FunctionSet::FunctionSet(std::string fp)
{
	std::string line;
	size_t lineNum = 1;
	std::ifstream stack_file(fp);

	if (!stack_file.is_open())
		throw std::runtime_error("failed to open " + fp);

	while (std::getline(stack_file, line)) {
		FuncInfo usage;

		if (!parseUsage(usage, line))
			throw ParserError(fp, lineNum, "invalid function stack usage");
		funcs.insert(std::make_pair<Address, FuncInfo>(
		       	(Address)usage.addr,
		       	FuncInfo(usage.name, usage.addr, usage.stack_size)
		));

		lineNum++;
	}
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
