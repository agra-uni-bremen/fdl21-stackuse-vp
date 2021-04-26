#ifndef RISCV_VP_RTOS_H
#define RISCV_VP_RTOS_H

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <string>
#include <systemc>
#include <memory>
#include <tlm_utils/simple_initiator_socket.h>

typedef uint16_t ThreadID;

class Thread {
public:
	ThreadID id;
	uint64_t stack_start;
	size_t stack_size;

	Thread(ThreadID _id, uint64_t _start, size_t _size)
	  : id(_id), stack_start(_start), stack_size(_size) {}
};

class RTOS : public sc_core::sc_module {
protected:
	void read_memory(void *buf, size_t bufsiz, uint64_t addr);

	RTOS(sc_core::sc_module_name name) : sc_module(name) {}
public:
	tlm_utils::simple_initiator_socket<RTOS> isock;

	virtual std::unique_ptr<Thread> thread_by_id(ThreadID) = 0;
	virtual std::unique_ptr<Thread> thread_by_stk(uint64_t) = 0;
};

#endif
