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

class Thread {
public:
	int id;
	uint64_t stack_start;
	size_t stack_size;

	Thread()
	  : id(0), stack_start(0), stack_size(0) {}

	Thread(int _id, uint64_t _start, size_t _size)
	  : id(_id), stack_start(_start), stack_size(_size) {}

	bool operator==(const Thread& rhs) const {
		return stack_start == rhs.stack_start;
	}
};

// From the std::hash documentation in the C++ reference.
class ThreadHash {
public:
    std::size_t operator()(Thread const& t) const noexcept {
        std::size_t h1 = std::hash<size_t>{}(t.stack_start);
        std::size_t h2 = std::hash<uint64_t>{}(t.stack_size);
        return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
    }
};

class RTOS : public sc_core::sc_module {
protected:
	void read_memory(void *buf, size_t bufsiz, uint64_t addr);

	RTOS(sc_core::sc_module_name name) : sc_module(name) {}
public:
	tlm_utils::simple_initiator_socket<RTOS> isock;

	virtual bool is_exit(std::string func_name) = 0;

	virtual std::unique_ptr<Thread> thread_by_id(int) = 0;
	virtual std::unique_ptr<Thread> thread_by_stk(uint64_t) = 0;
};

#endif
