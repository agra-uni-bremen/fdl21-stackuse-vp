#include "stack_usage.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <regex>

#include <string>
#include <system_error>

/* Regular expression for gcc -fstack-usage file format */
#define REG "..*:[0-9][0-9]*:[0-9][0-9]*:(..*)	([0-9][0-9]*)	([a-z,]*)"
#define SUB 4 /* match includes entire string */

static uint8_t
parseTypes(std::string input)
{
	/* XXX: This parser is not as strict as it could be.
	 * Currently, it only checks for substrings. */

	if (input.find("static") != std::string::npos)
		return FSTACK_STATIC;

	uint8_t r = 0;
	if (input.find("dynamic") != std::string::npos)
		r |= FSTACK_DYNAMIC;
	if (input.find("bounded") != std::string::npos)
		r |= FSTACK_BOUNDED;

	return r;
}

static ssize_t
parseSize(std::string input)
{
	unsigned long long r;

	/* reset errno for error check */
	errno = 0;

	r = strtoull(input.c_str(), NULL, 10);
	if (errno != 0)
		return -1;

	assert(r <= SSIZE_MAX);
	return (ssize_t)r;
}

static bool
parseUsage(FuncStackUsage &usage, std::string line)
{
	std::regex re(REG);

	std::smatch match;
	if (regex_search(line, match, re)) {
		if (match.size() != SUB)
			return false;

		auto name = match[1].str();
		auto size = parseSize(match[2].str());
		if (size == -1)
			return false;

		auto types = parseTypes(match[3].str());
		if (types == 0)
			return false;

		usage.name = name;
		usage.stack_usage = size;
		usage.types = types;

		return true;
	}

	return false;
}

StackUsage::StackUsage(std::string name, std::ifstream &stream)
{
	std::string line;
	size_t lineNum = 1;

	while (std::getline(stream, line)) {
		FuncStackUsage usage;

		if (!parseUsage(usage, line))
			throw ParserError(name, lineNum, "invalid function stack usage");
		if (!((usage.types & FSTACK_BOUNDED) || (usage.types & FSTACK_STATIC)))
			throw std::invalid_argument("unbounded stack usage in " + usage.name);

		assert(!funcs.count(usage.name));
		funcs[usage.name] = usage.stack_usage;

		lineNum++;
	}
}

size_t
StackUsage::get_usage(std::string func_name)
{
	return funcs.at(func_name);
}
