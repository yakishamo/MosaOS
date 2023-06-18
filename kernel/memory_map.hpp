#pragma once

typedef struct MemoryMap {
	unsigned long long buffer_size;
	void *buffer;
	unsigned long long map_size;
	unsigned long long map_key;
	unsigned long long descriptor_size;
	uint32_t descriptor_version;
} MemoryMap_t;

typedef struct MemoryDescriptor {
	uint32_t type;
	uintptr_t physical_start;
	uintptr_t virtual_start;
	uint64_t number_of_pages;
	uint64_t attribute;
} MemoryDescriptor_t;

#ifdef __cplusplus
typedef enum class MemoryType {
	kEfiReservedMemoryType,
	kEfiLoaderCode,
	kEfiLoaderData,
	kEfiBootServicesCode,
	kEfiBootServicesData,
	kEfiRuntimeServicesCode,
	kEfiRuntimeServicesData,
	kEfiConventionalMemory,
	kEfiUnusableMemory,
	kEfiACPIReclaimMemory,
	kEfiACPIMemoryNVS,
	kEfiMemoryMappedIO,
	kEfiMemoryMappedIOPortSpace,
	kEfiPalCode,
	kEfiPersistentMemory,
	kEfiMaxMemoryType
} MemoryType_t;

inline bool operator==(uint32_t lhs, MemoryType rhs) {
	return lhs == static_cast<uint32_t>(rhs);
}

inline bool operator==(MemoryType lhs, uint32_t rhs) {
	return rhs == lhs;
}

inline bool IsAvailable(MemoryType_t memory_type) {
	return
		memory_type == MemoryType_t::kEfiBootServicesCode ||
		memory_type == MemoryType_t::kEfiBootServicesData ||
		memory_type == MemoryType_t::kEfiConventionalMemory;
}

const int kUEFIPageSize = 4096;

#endif /* __cplusplus */


