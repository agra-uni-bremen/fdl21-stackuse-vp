#ifndef RISCV_VP_RTOS_RIOT_H
#define RISCV_VP_RTOS_RIOT_H

#include <string>
#include <unordered_map>

#include "elf_file.h"
#include "rtos.h"

class RIOT : public RTOS {
private:
	uint8_t maxthrs;
	uint32_t baseaddr;

	uint8_t startoff;
	uint8_t sizoff;

	std::vector<Thread> threads;
	void update_threads(void);

public:
	RIOT(void);
	~RIOT(void);

	void init(std::string fp);

	std::unique_ptr<Thread> thread_by_id(ThreadID);
	std::unique_ptr<Thread> thread_by_stk(uint64_t);
};

#endif
