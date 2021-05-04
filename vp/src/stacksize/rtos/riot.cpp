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

	/* Offset of name field in _thread struct.
	 * Not strictly required, but useful for debugging.
	 * XXX: Requires compilation with DEVELHELP. */
	auto nameaddr = elf.get_symbol("_tcb_name_offset");
	read_memory(&nameoff, sizeof(nameoff), nameaddr);

	isr.name = "ISR";
	isr.id = THREAD_ID_STK;
	isr.stack_start = elf.get_symbol("_eheap");
	isr.stack_size = elf.get_symbol("__stack_size");

	baseaddr = elf.get_symbol("sched_threads");
}

void
RIOT::update_threads(void) {
	threads.clear(); // Remove all previously recorded threads.
	threads.push_back(isr);

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

		uint32_t nameaddr;
		read_memory(&nameaddr, sizeof(nameaddr), tcbptr + nameoff);

		threads.push_back(Thread(i, stkstart, stksiz, read_string(nameaddr)));
	}
}

bool
RIOT::is_isr_stk(uint64_t addr) {
	auto end = isr.stack_start + isr.stack_size;
	return addr > isr.stack_start && addr <= end;
}

std::unique_ptr<Thread>
RIOT::thread_by_id(int id) {
	if (id == THREAD_ID_STK)
		return std::make_unique<Thread>(isr);

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
		return std::make_unique<Thread>(isr);
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
