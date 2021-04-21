#ifndef RISCV_VP_RTOS_H
#define RISCV_VP_RTOS_H

#include <stdint.h>
#include <stddef.h>

#include <string>

typedef uint64_t ThreadID;

class Thread {
public:
	ThreadID get_id(void);
	size_t stack_size(void);
};

class RTOS {
public:
	virtual std::string get_name(void) = 0;

	virtual ThreadID get_active_thread(void) = 0;
	virtual Thread &get_thread(ThreadID) = 0;
};

#endif
