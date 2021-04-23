#include "riot.h"

enum {
	RIOT_THREADS_BASE = 0,
	RIOT_NUM_THREADS,
	RIOT_ACTIVE_PID,
	RIOT_MAX_THREADS,
	RIOT_NAME_OFFSET,
};

/* see core/sched.c in the RIOT source tree*/
static std::string riot_symbols[] = {
	"sched_threads",
	"sched_num_threads",
	"sched_active_pid",
	"max_threads",
	"_tcb_name_offset",
};

RIOT::RIOT(std::string fp) : RTOS("RIOT"), elf(fp) {
	return;
}

RIOT::~RIOT(void) {
	return;
}

std::string
RIOT::get_name(void) {
	return "RIOT";
}

ThreadID
RIOT::get_active_thread(void) {
	auto symname = riot_symbols[RIOT_ACTIVE_PID];
	auto symaddr = elf.get_symbol(symname);

	ThreadID id;
	read_memory(&id, sizeof(id), symaddr);

	return id;
}

Thread &
RIOT::get_thread(ThreadID id) {
	throw "not implemented";
}
