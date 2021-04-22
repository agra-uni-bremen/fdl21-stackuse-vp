#ifndef RISCV_VP_RTOS_H
#define RISCV_VP_RTOS_H

#include <stdint.h>
#include <stddef.h>

#include <string>
#include <systemc>
#include <vector>
#include <tlm_utils/simple_initiator_socket.h>

typedef uint16_t ThreadID;

class Thread {
public:
	ThreadID get_id(void);
	size_t stack_size(void);
};

class RTOS : public sc_core::sc_module {
protected:
	void read_memory(void *buf, size_t bufsiz, uint64_t addr);
public:
	tlm_utils::simple_initiator_socket<RTOS> isock;

	virtual std::string get_name(void) = 0;

	virtual ThreadID get_active_thread(void) = 0;
	virtual Thread &get_thread(ThreadID) = 0;
};

#endif
