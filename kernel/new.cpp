#include "new.hpp"

#include <stddef.h>

void *operator new(size_t size, void *buf) {
	return buf;
}

void operator delete(void *obj) noexcept {
}


