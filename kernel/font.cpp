#include "font.hpp"
#include "new.hpp"

MonoAsciiFont::MonoAsciiFont() {
	for(int i = 0; i < row_num; i++) {
		row[i] = 0;
	}
}

bool MonoAsciiFont::isDraw(unsigned int row, unsigned int column) {
	if(row < row_num && column < sizeof(uint8_t)) {
		return ((this->row[row] >> (7-column)) & 1) == 1;
	}
	return false;
}

MonoAsciiFont *font_ptr_buf[font_num];
MonoAsciiFont font_buf[font_num];

AsciiFont::AsciiFont(){
	font = new(font_ptr_buf) MonoAsciiFont*;
	for(int i = 0; i < font_num; i++) {
		font[i] = new(font_buf + i) MonoAsciiFont();
	}
}

