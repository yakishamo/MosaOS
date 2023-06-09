#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../boot/boot_types.h"
#include "screen.hpp"

class Screen;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} __attribute__((packed)) Color_t;

#include "bmp.hpp"

typedef void (*WritePixel_t)(uint32_t x, uint32_t y, Color_t c);

class ScreenManager {
	public:
		ScreenManager();
		ScreenManager(Screen *s);
		Screen *getScreen(int index);
		Screen *setScreen(Screen *s);
		bool copyScreen(Screen *dst, Screen *src);
	private:
		Screen *screens;
		Screen *getLast();
};

class Screen {
	public:
		Screen();
		Screen(const video_info_t *vinfo, const char *name);
		Screen *setNext(Screen *next);
		Screen *getNext();
		char *setName(const char *str);
		char *getName();
		uint32_t getX();
		uint32_t getY();
		Screen *writePixel(uint32_t x, uint32_t y, Color_t c);
		Screen *writeSquare(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, Color_t c);
		Screen *writeAscii(uint32_t x, uint32_t y, Color_t coler, char c);
		Screen *printLine(uint32_t x, uint32_t y, Color_t c, const char *line);
		Screen *printBmp(uint32_t x, uint32_t y, BitMapImage *bmp);
		uint32_t *getFb();
		uint64_t getFbsize();
	private:
		char name[100];
		uint32_t *fb;
		uint64_t fb_size;
		uint32_t x_axis;
		uint32_t y_axis;
		uint32_t ppsl;
		Screen *next;
};

Screen *copyScreen(Screen *dst, Screen *src);
ScreenManager *InitializeScreen(const video_info_t *vinfo);
