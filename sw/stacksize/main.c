#include <stdint.h>

static volatile uint64_t *MTIME_REG = (uint64_t *)0x200bff8;

int myfunc2(uint64_t a) {
	int b = *MTIME_REG;
	b *= 2;

	return a + b;
}

int myfunc1(void) {
	uint64_t a = *MTIME_REG;
	return myfunc2(a);
}

int main(void) {
	volatile int r = myfunc1();
	r * 2;

	return r;
}
