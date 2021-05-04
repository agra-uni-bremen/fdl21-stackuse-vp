#include "rtos.h"

#include <sstream>

void
RTOS::read_memory(void *buf, size_t bufsiz, uint64_t addr)
{
	tlm::tlm_generic_payload trans;

	trans.set_command(tlm::TLM_READ_COMMAND);
	trans.set_address(addr);
	trans.set_data_ptr((uint8_t*)buf);
	trans.set_data_length(bufsiz);
	trans.set_response_status(tlm::TLM_OK_RESPONSE);

	unsigned ret = isock->transport_dbg(trans);
	if (trans.is_response_error())
		throw std::runtime_error("debug transaction failed");
	else if (ret != bufsiz)
		throw std::runtime_error("short write");
}

std::string
RTOS::read_string(uint64_t addr)
{
	char c;
	std::ostringstream stream;

	do {
		read_memory(&c, sizeof(c), addr++);
		stream << c;
	} while (c != '\0');

	return stream.str();
}
