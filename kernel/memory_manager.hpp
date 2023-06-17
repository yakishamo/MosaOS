#pragma once
#include "memory_map.h"

namespace{
	constexpr unsigned long long operator""_KiB(unsigned long long kib) {
		return kib*1024;
	}
	constexpr unsigned long long operator""_MiB(unsigned long long mib) {
		return mib*1024_KiB;
	}
	constexpr unsigned long long operator""_GiB(unsigned long long gib) {
		return gib*1024_GiB;
	}
}

static const auto kBytesPerFrame{4_KiB};

class FrameID {
	public:
		explicit FrameID(size_t id) : id_{id}{}
		size_t ID() const { return id_ }
		void *Frame() const {return reinterpret_cast<void*>(id_ * kBytesPerFrame);}

	private:
		size_t id_;
};

static const FrameID kNullFrame{std::numeric_limits<size_t>::max()};

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
