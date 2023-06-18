#define MEMORY_MAP_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include "../boot/boot_types.h"
#include "kernel.hpp"
#include "asmfunc.hpp"
#include "font.hpp"
#include "screen.hpp"
#include "new.hpp"
#include "segment.hpp"
#include "paging.hpp"
#include "memory_manager.hpp"
#include "memory_map.hpp"
#include "interrupt.hpp"
#include "bmp.hpp"

video_info_t *vinfo;
extern AsciiFont *gFont;
char font_buf[sizeof(AsciiFont)];
Screen *gScreen = NULL;
alignas(16) uint8_t kernel_main_stack[1024*1024];
BitmapMemoryManager *gMemoryManager;
char memory_manager_buf[sizeof(BitmapMemoryManager)];
char bmp_buf[sizeof(BitMapImage)];

extern "C" void KernelMain(bootinfo_t *binfo) {
	vinfo = &binfo->vinfo;
	gFont = new(font_buf) AsciiFont(binfo->font);
	const MemoryMap *memory_map = binfo->mmap;

	ScreenManager *screen_manager = InitializeScreen((const video_info_t *)vinfo);
	Screen *frame = screen_manager->getScreen(0);
	gScreen = frame;
	uint32_t frame_x = frame->getX();
	uint32_t frame_y = frame->getY();
	for(uint32_t i=0; i < frame_x; i++) {
		for(uint32_t j=0; j < frame_y; j++) {
			frame->writePixel(i, j, {0xff, 0xff, 0xff});
		}
	}

	frame->writeSquare(100,100,200,200,{0,0xff,0})
		->writeSquare(150,150,170,230,{0,0,0xff});

	print("Hello, World!!");
	print("Welcome to MosaOS!!!");

	print("Setup Segments.");
	SetupSegments();
	const uint16_t kernel_cs = 1 << 3;
	const uint16_t kernel_ss = 2 << 3;
	SetDSAll(0);
	SetCSSS(kernel_cs, kernel_ss);
	print("finished.");

	print("Setup Paging.");
	SetupIdentityPageTable();
	print("finished.");

	print("init interrupt");
	InitializeInterrupt();
	print("finished.");

	print("print Mosa.bmp");
	frame->printBmp(200,200,
			new(bmp_buf) BitMapImage(reinterpret_cast<uintptr_t>(binfo->bmp)));

	__asm__("int3");

////////////////////////////////////////////////////////////////////////////////////////////

	print("Setup Memory Manager.");
	::gMemoryManager = 
		new(reinterpret_cast<BitmapMemoryManager*>(memory_manager_buf)) BitmapMemoryManager;
	const auto memory_map_base = reinterpret_cast<uintptr_t>(memory_map->buffer);
	uintptr_t available_end = 0;
	unsigned int num_memdesc = memory_map->map_size / memory_map-> descriptor_size;
	/*
	for(uintptr_t iter = memory_map_base;
			iter < memory_map_base + memory_map->map_size;
			iter += memory_map->descriptor_size) { */
	uintptr_t iter = memory_map_base;
	for(unsigned int i=0; i < num_memdesc; i++ ) {
		auto desc = reinterpret_cast<const MemoryDescriptor_t*>(iter);
		if(available_end < desc->physical_start) {
			gMemoryManager->MarkAllocated(
					FrameID{available_end / kBytesPerFrame},
					(desc->physical_start - available_end) / kBytesPerFrame);
		}

		const auto physical_end = 
			desc->physical_start + desc->number_of_pages * kUEFIPageSize;
		if(IsAvailable(static_cast<MemoryType_t>(desc->type))) {
			available_end = physical_end;
			//print("available");
		} else {
			print("un-available");
			gMemoryManager->MarkAllocated(
					FrameID{desc->physical_start / kBytesPerFrame},
					desc->number_of_pages * kUEFIPageSize / kBytesPerFrame);
		}
		iter += memory_map->descriptor_size;
	}
	printd("for loop finished.");
	gMemoryManager->SetMemoryRange(FrameID{1}, FrameID{available_end / kBytesPerFrame});
	print("finished.");
		
	if(available_end/kBytesPerFrame > 64) {
		print("over 64");
	} else {
		print("less than 64");
	}

	print("init heap.");
	if(auto err = InitializeHeap(*gMemoryManager)) {
		printd("InitializeHeap failed.");
		printd(err.Name());
		while(1) __asm__("hlt");
	}
	print("finished.");

	while(1) {
		__asm__("hlt");
	}
}

void printd(const char *line, ...) {
	/*
	va_list args;
	__builtin_va_start(args, line);
	char str[strlen(line)+100];
	vsprintf(str, line, args);
	*/
	static int y = 0;
	gScreen->writeSquare(0,y,strlen(line)*8,y+16,{0,0,0})
		->printLine(0,y,{0xff,0,0}, line);
	y+=16;
	// __builtin_va_end(args);
	return;
}

void print(const char *line, ...) {
	static int y = 0;
	static uint8_t c = 0;
	gScreen->writeSquare(0,y,strlen(line)*8, y+16, {0xff,0xff,0xff})
		->printLine(0,y,{c,0,0}, line);
	y+=16;
	if(y > gScreen->getY()) {
		y = 0;
		c += 0x10;
	}
	return;
}
