#include "screen.hpp"
#include "new.hpp"
#include "../boot/boot_types.h"
#include <cstddef>
#include <cstring>

ScreenManager::ScreenManager() {
	screens = NULL;
	return;
}

ScreenManager::ScreenManager(Screen *s) {
	setScreen(s);
	return;
}

Screen *ScreenManager::getScreen(int index) {
	Screen *ptr = this->screens;
	for(int i = 0; i < index; i++) {
		if(ptr == NULL) {
			return NULL;
		} else {
			ptr = ptr->getNext();
		}
	}
	return ptr;
}

Screen *ScreenManager::setScreen(Screen *s) {
	Screen *ptr = getLast();
	if(ptr == NULL) {
		screens = s;
		return s;
	} else {
		ptr->setNext(s);
		return s;
	}
}

Screen *ScreenManager::getLast() {
	if(screens->getNext() == NULL) {
		return NULL;
	}
	Screen *ptr = screens;
	while(ptr->getNext() != NULL) {
		ptr = ptr->getNext();
	}
	return ptr;
}

Screen::Screen() {
	fb = NULL;
	fb_size = 0;
	x_axis = 0;
	y_axis = 0;
	ppsl = 0;
	next = NULL;
}

Screen::Screen(video_info_t vinfo, const char *name) {
	while(1)__asm__("hlt");
	fb = vinfo.fb;
	fb_size = vinfo.fb_size;
	x_axis = vinfo.x_axis;
	y_axis = vinfo.y_axis;
	ppsl = vinfo.ppsl;
	next = NULL;
	setName(name);
}

Screen *Screen::setNext(Screen *next) {
	this->next = next;
	return next;
}

Screen *Screen::getNext() {
	return this->next;
}

char *Screen::setName(const char *str) {
	while(1) __asm__("hlt");
	strncpy(name, str, 99);
	name[99] = '\0';
	return name;
}

char *Screen::getName() {
	return name;
}

ScreenManager screen_manager_buf;
Screen framebuffer_buf, backbuffer_buf;
uint64_t back_fb[2116800];

ScreenManager *InitializeScreen(video_info_t vinfo) {
	ScreenManager *screen_manager = new(&screen_manager_buf) ScreenManager(
			new(&framebuffer_buf) Screen(vinfo, "FrameBuffer"));
		
	video_info_t back_info;
	memcpy(&back_info, &vinfo, sizeof(video_info_t));
	back_info.fb = back_fb;
	Screen *back_buffer = new(&backbuffer_buf) Screen(back_info, "BackBuffer");
	screen_manager->setScreen(back_buffer);
	return screen_manager;
}

