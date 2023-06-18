#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

#include "memory_map.h"
#include <stdint.h>

typedef struct MemoryMap MemoryMap_t;

typedef struct video_info_t {
	uint32_t *fb;
	uint64_t fb_size;
	uint32_t x_axis;
	uint32_t y_axis;
	uint32_t ppsl;
} video_info_t;

typedef struct bootinfo_t {
	video_info_t vinfo;
	uint64_t *font;
	MemoryMap_t *mmap;
} bootinfo_t;

#endif /* BOOT_TYPES_H */
