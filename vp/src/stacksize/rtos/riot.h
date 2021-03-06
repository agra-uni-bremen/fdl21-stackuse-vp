#ifndef RISCV_VP_RTOS_RIOT_H
#define RISCV_VP_RTOS_RIOT_H

#include <string>
#include <unordered_map>

#include "elf_file.h"
#include "rtos.h"

#define THREAD_ID_STK -1

class RIOT : public RTOS {
private:
	uint8_t maxthrs;
	uint32_t baseaddr;

	uint8_t startoff;
	uint8_t sizoff;
	uint8_t nameoff;

	/* ISR stack */
	Thread isr;

	std::vector<Thread> threads;

	void update_threads(void);
	bool is_isr_stk(uint64_t);

public:
	RIOT(void);
	~RIOT(void);

	void init(std::string fp);

	std::unique_ptr<Thread> thread_by_id(int);
	std::unique_ptr<Thread> thread_by_stk(uint64_t);
};

#endif
