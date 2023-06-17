#include "font.hpp"
#include "new.hpp"
#include <cstring>

MonoAsciiFont::MonoAsciiFont(uint64_t *mono_font) {
	memcpy(row, mono_font, row_num);
}

bool MonoAsciiFont::isDraw(int x, int y) {
	if(x < 8 && y < 16) {
		if(((row[y] >> (7-x))&1) == 1){
			return true;
		}
	} else {
		while(1)__asm__("hlt");
	}
	return false;
}

char mono_font_buf[font_num*sizeof(MonoAsciiFont)];

AsciiFont::AsciiFont(uint64_t *font_file){
	for(int i = 0; i < font_num; i++) {
		this->font[i] = new(reinterpret_cast<MonoAsciiFont*>(mono_font_buf) + i)
			MonoAsciiFont(font_file + i*2);
	}
}

MonoAsciiFont *AsciiFont::getFont(int c) {
	return this->font[c];
}
