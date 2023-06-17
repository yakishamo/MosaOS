#include <stdint.h>
#include <stddef.h>
#include "../boot/boot_types.h"
#include "kernel.hpp"
#include "font.hpp"
#include "screen.hpp"

video_info_t *vinfo;

extern "C" void entry_point(bootinfo_t *binfo) {
	vinfo = &binfo->vinfo;
	uint32_t x_axis = vinfo->x_axis;
	uint32_t y_axis = vinfo->y_axis;
	uint32_t ppsl = binfo->vinfo.ppsl;
	uint32_t *fb = (uint32_t*)binfo->vinfo.fb;
	uint32_t fb_size = vinfo->fb_size;

	for(uint32_t i=0; i < fb_size/4; i++) {
		fb[i] = 0xffffffff;
	}

	ScreenManager *screen_manager = InitializeScreen((const video_info_t *)vinfo);
	Screen *frame = screen_manager->getScreen(0);
	uint32_t frame_x = frame->getX();
	uint32_t frame_y = frame->getY();
	for(uint32_t i=0; i < frame_x; i++) {
		for(uint32_t j=0; j < frame_y; j++) {
			frame->writePixel(i, j, {0x80, 0x0, 0x0});
		}
	}

	while(1) {
		__asm__("hlt");
	}
}
