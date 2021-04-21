#ifndef RISCV_VP_RTOS_RIOT_H
#define RISCV_VP_RTOS_RIOT_H

#include <string>
#include <unordered_map>

#include "rtos.h"

class RIOT : public RTOS {
private:
	std::unordered_map<ThreadID, Thread> threads;

public:
	RIOT(std::string elf);
	~RIOT(void);

	std::string get_name(void) override;
	ThreadID get_active_thread(void) override;
	Thread &get_thread(ThreadID) override;
};

#endif
