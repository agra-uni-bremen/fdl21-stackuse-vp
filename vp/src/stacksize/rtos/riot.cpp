#include "riot.h"

enum {
	FIRST = 1,
	SECOND
};

static inline bool
contains(uint64_t start, size_t size, uint64_t addr) {
	return addr >= start && addr < (start + size);
}

RIOT::RIOT(void) : RTOS("RIOT") {
	return;
}

RIOT::~RIOT(void) {
	return;
}

void
RIOT::init(std::string fp) {
	ELFFile elf(fp);

	auto maxaddr = elf.get_symbol("max_threads");
	read_memory(&maxthrs, sizeof(maxthrs), maxaddr);

	/* Offset of stack_start field in _thread struct.
	 * XXX: Requires compilation with DEVELHELP. */
	auto spaddr = elf.get_symbol("_tcb_stack_start_offset");
	read_memory(&startoff, sizeof(startoff), spaddr);

	/* Offset of stack_size field in _thread struct.
	 * XXX: Requires compilation with DEVELHELP. */
	auto sizaddr = elf.get_symbol("_tcb_stack_size_offset");
	read_memory(&sizoff, sizeof(sizoff), sizaddr);

	/* ISR stack */
	_eheap = elf.get_symbol("_eheap");
	__stack_size = elf.get_symbol("__stack_size");

	baseaddr = elf.get_symbol("sched_threads");
}

bool
RIOT::is_exit(std::string func_name) {
	return func_name == "exit";
}

void
RIOT::update_threads(void) {
	threads.clear(); // Remove all previously recorded threads.

	for (size_t i = 0; i < maxthrs; i++) {
		uint32_t tcbptr;

		uint32_t addr = baseaddr + i * sizeof(tcbptr);
		read_memory(&tcbptr, sizeof(tcbptr), addr);
		if (tcbptr == 0)
			continue; /* unused */

		uint32_t stkstart;
		read_memory(&stkstart, sizeof(stkstart), tcbptr + startoff);

		int32_t stksiz; /* technically an RV32 C int */
		read_memory(&stksiz, sizeof(stksiz), tcbptr + sizoff);

		threads.push_back(Thread(i, stkstart, stksiz));
	}
}

bool
RIOT::is_isr_stk(uint64_t addr) {
	// Stack starts at _eheap + __stack_size and grows
	// downward towards the address specified by _eheap.
	return addr > _eheap && addr <= (_eheap + __stack_size);
}

std::unique_ptr<Thread>
RIOT::thread_by_id(int id) {
	for (int run : {FIRST, SECOND}) {
		for (auto t : threads) {
			if (t.id == id)
				return std::make_unique<Thread>(t);
		}

		// Thread not found on first run, update list and try again.
		if (run == FIRST)
			update_threads();
	}

	return nullptr;
}

std::unique_ptr<Thread>
RIOT::thread_by_stk(uint64_t stkptr) {
	if (is_isr_stk(stkptr))
		return nullptr;
	else if (stkptr == 0)
		return nullptr; /* initial value, no need to update thread list */

	for (int run : {FIRST, SECOND}) {
		for (auto t : threads) {
			if (contains(t.stack_start, t.stack_size, stkptr))
				return std::make_unique<Thread>(t);
		}

		// Thread not found on first run, update list and try again.
		if (run == FIRST)
			update_threads();
	}

	return nullptr;
}
