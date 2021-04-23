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

RIOT::RIOT(std::string fp) : RTOS("RIOT") {
	ELFFile elf(fp);

	auto maxsym = riot_symbols[RIOT_MAX_THREADS];
	auto maxaddr = elf.get_symbol(maxsym);
	read_memory(&maxthrs, sizeof(maxthrs), maxaddr);

	auto basesym = riot_symbols[RIOT_THREADS_BASE];
	baseaddr = elf.get_symbol(basesym);

	return;
}

RIOT::~RIOT(void) {
	return;
}

void
RIOT::update_threads(void) {
	// Remove all previously recorded threads.
	threads.clear();

	for (size_t i = 0; i < threads.size(); i++) {
		uint32_t tcbptr;

		uint64_t addr = baseaddr + i * sizeof(tcbptr);
		read_memory(&tcbptr, sizeof(tcbptr), addr);

		if (tcbptr == 0)
			continue; /* unused */

		threads.push_back(Thread(i, 0, 0));
	}
}

std::unique_ptr<Thread>
RIOT::find_thread(ThreadID id) {
	update_threads();

	for (auto t : threads) {
		if (t.id == id)
			return std::make_unique<Thread>(t);
	}

	return nullptr;
}

std::unique_ptr<Thread>
RIOT::find_thread(uint64_t stkptr) {
	update_threads();

	for (auto t : threads) {
		if (t.stack_start >= stkptr && stkptr <= (t.stack_start + t.stack_size))
			return std::make_unique<Thread>(t);
	}

	return nullptr;
}
