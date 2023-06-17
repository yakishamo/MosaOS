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

video_info_t *vinfo;
extern AsciiFont *gFont;
char font_buf[sizeof(AsciiFont)];
Screen *gScreen = NULL;
alignas(16) uint8_t kernel_main_stack[1024*1024];

extern "C" void KernelMain(bootinfo_t *binfo) {
	vinfo = &binfo->vinfo;
	gFont = new(font_buf) AsciiFont(binfo->font);

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
	/*
	va_list args;
	__builtin_va_start(args, line);
	char str[strlen(line)+100];
	vsprintf(str, line, args);
	*/
	static int y = 0;
	gScreen->printLine(0,y,{0,0,0}, line);
	y+=16;
	// __builtin_va_end(args);
	return;
}
