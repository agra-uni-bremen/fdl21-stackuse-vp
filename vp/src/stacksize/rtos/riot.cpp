#include "riot.h"

RIOT::RIOT(std::string fp) : RTOS("RIOT") {
	ELFFile elf(fp);

	auto maxaddr = elf.get_symbol("max_threads");
	read_memory(&maxthrs, sizeof(maxthrs), maxaddr);

	/* Offset of sp field in _thread struct.
	 * XXX: Requires compilation with DEVELHELP. */
	auto spaddr = elf.get_symbol("_tcb_sp_offset");
	read_memory(&spoff, sizeof(spoff), spaddr);

	/* Offset of stack_size field in _thread struct.
	 * XXX: Requires compilation with DEVELHELP. */
	auto sizaddr = elf.get_symbol("_tcb_stack_size_offset");
	read_memory(&sizoff, sizeof(sizoff), sizaddr);

	baseaddr = elf.get_symbol("sched_threads");
	return;
}

RIOT::~RIOT(void) {
	return;
}

void
RIOT::update_threads(void) {
	threads.clear(); // Remove all previously recorded threads.

	for (size_t i = 0; i < threads.size(); i++) {
		uint32_t tcbptr;

		uint32_t addr = baseaddr + i * sizeof(tcbptr);
		read_memory(&tcbptr, sizeof(tcbptr), addr);
		if (tcbptr == 0)
			continue; /* unused */

		uint32_t sp;
		read_memory(&sp, sizeof(sp), tcbptr + spoff);

		int32_t stksiz; /* technically an RV32 C int */
		read_memory(&stksiz, sizeof(stksiz), tcbptr + sizoff);

		threads.push_back(Thread(i, sp, stksiz));
	}
}

std::unique_ptr<Thread>
RIOT::find_thread(ThreadID id) {
	/* TODO: Heuristic for updating the thread list */
	update_threads();

	for (auto t : threads) {
		if (t.id == id)
			return std::make_unique<Thread>(t);
	}

	return nullptr;
}

std::unique_ptr<Thread>
RIOT::find_thread(uint64_t stkptr) {
	/* TODO: Heuristic for updating the thread list */
	update_threads();

	for (auto t : threads) {
		if (t.stack_start >= stkptr && stkptr <= (t.stack_start + t.stack_size))
			return std::make_unique<Thread>(t);
	}

	return nullptr;
}
