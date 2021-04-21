#include "riot.h"

RIOT::RIOT(std::string elf) {
	return;
}

RIOT::~RIOT(void) {
	return;
}

std::string
RIOT::get_name(void) {
	return "RIOT";
}

ThreadID
RIOT::get_active_thread(void) {
	throw "not implemented";
}

Thread &
RIOT::get_thread(ThreadID id) {
	throw "not implemented";
}
