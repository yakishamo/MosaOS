#ifndef NEW_HPP
#define NEW_HPP

#include <stddef.h>

void *operator new(size_t size, void *buf);

void operator delete(void *obj) noexcept;

#endif /* NEW_HPP */
