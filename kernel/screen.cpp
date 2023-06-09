#include "screen.hpp"
#include "../boot/boot_types.h"
#include <stddef.h>
#include "new.hpp"

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

Screen::Screen(video_info_t vinfo) {
	fb = vinfo.fb;
	fb_size = vinfo.fb_size;
	x_axis = vinfo.x_axis;
	y_axis = vinfo.y_axis;
	ppsl = vinfo.ppsl;
	next = NULL;
}

Screen *Screen::setNext(Screen *next) {
	this->next = next;
	return next;
}

Screen *Screen::getNext() {
	return this->next;
}

ScreenManager screen_manager_buf;
Screen screen_buf;

ScreenManager *InitializeScreen(video_info_t vinfo) {
	ScreenManager *screen_manager = new(&screen_manager_buf) ScreenManager(
			new(&screen_buf) Screen(vinfo));
	return screen_manager;
}

