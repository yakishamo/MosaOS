#include "kernel.hpp"
#include "memory_manager.hpp"
#include "memory_map.h"
#include "screen.hpp"

MemoryManager::MemoryManager(MemoryMap_t *mmap) {
	this->mmap = mmap;
	this->memdesc = reinterpret_cast<MemoryDescriptor_t>(mmap->buffer);
	return;
}

