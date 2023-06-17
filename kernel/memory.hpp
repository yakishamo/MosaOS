#pragma once
#include "memory_map.h"

inline bool IsAvailable(MemoryType_t memory_type) {
	return
		memory_type == kEfiBootServicesCode ||
		memory_type == kEfiBootServicesData ||
		memory_type == kEfiConventionalMemory;
}

const int kUefiPageSize = 4096;

class MemoryManager{
	public:
		MemoryManager(MemoryMap_t *mmap);
	private:
		MemoryMap_t *mmap;
		MemoryDescriptor *memdesc;
}

