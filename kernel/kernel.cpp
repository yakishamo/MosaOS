#include <cstdint>
#include <cstddef>
#include <cstring>
#include "../boot/boot_types.h"
#include "kernel.hpp"
#include "font.hpp"
#include "screen.hpp"
#include "new.hpp"

video_info_t *vinfo;
extern AsciiFont *gFont;
char font_buf[sizeof(AsciiFont)];
Screen *gScreen = NULL;

void printd(const char *line);

extern "C" void entry_point(bootinfo_t *binfo) {
	vinfo = &binfo->vinfo;
	uint32_t x_axis = vinfo->x_axis;
	uint32_t y_axis = vinfo->y_axis;
	uint32_t ppsl = binfo->vinfo.ppsl;
	uint32_t *fb = (uint32_t*)binfo->vinfo.fb;
	uint32_t fb_size = vinfo->fb_size;

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

	frame->printLine(0,0,{0x00,0x00,0x00},"Hello, World!!")
		->printLine(0, 16, {0x00,0x00,0x00}, "Welcome to MosaOS!!!")
		->writeSquare(100,100,200,200,{0,0xff,0})
		->writeSquare(150,150,170,230,{0,0,0xff});

	printd("This is Debug Message");

	while(1) {
		__asm__("hlt");
	}
}

void printd(const char *line) {
	static int y = 0;
	gScreen->writeSquare(0,y,strlen(line)*8,y+16,{0,0,0})
		->printLine(0,y,{0xff,0,0}, line);
	y+=16;
	return;
}
