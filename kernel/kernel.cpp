#include <stdint.h>
#include <stddef.h>
#include "../boot/boot_types.h"
#include "kernel.hpp"

video_info_t *vinfo;

extern "C" void entry_point(bootinfo_t *binfo) {
	vinfo = &binfo->vinfo;
	uint32_t x_axis = vinfo->x_axis;
	uint32_t y_axis = vinfo->y_axis;
	uint32_t ppsl = binfo->vinfo.ppsl;
	pix_format *fb = (pix_format*)binfo->vinfo.fb;

	for(uint32_t i = 0; i < y_axis; i++) {
		for(uint32_t j = 0; j < x_axis; j++) {
			pix_format bgr = {
				static_cast<uint8_t>(i%0xff), 
				static_cast<uint8_t>(j%0xff),
				static_cast<uint8_t>((i*j)%0xff), 0x00};
			fb[i*ppsl+j] = bgr;
		}
	}

	while(1) {
		__asm__("hlt");
	}
}
