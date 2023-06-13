#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../boot/boot_types.h"

class Screen;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t res;
} __attribute__((packed)) Color_t;

typedef void (*WritePixel_t)(uint32_t x, uint32_t y, Color_t c);

class ScreenManager {
	public:
		ScreenManager();
		ScreenManager(Screen *s);
		Screen *getScreen(int index);
		Screen *setScreen(Screen *s);
	private:
		Screen *screens;
		Screen *getLast();
};

class Screen {
	public:
		Screen();
		Screen(video_info_t vinfo, const char *name);
		Screen *setNext(Screen *next);
		Screen *getNext();
		char *setName(const char *str);
		char *getName();
	private:
		char name[100];
		uint64_t *fb;
		uint64_t fb_size;
		uint32_t x_axis;
		uint32_t y_axis;
		uint32_t ppsl;
		Screen *next;
};

ScreenManager *InitializeScreen(video_info_t vinfo);
