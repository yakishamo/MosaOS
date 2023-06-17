#pragma once
#include <stdint.h>

const int row_num = 16;
const int font_num = 128;

class MonoAsciiFont {
	public:
		MonoAsciiFont(uint64_t *mono_font);		
		bool isDraw(int x, int y);

	private:
		unsigned char row[row_num];
};

class AsciiFont {
	public:
		AsciiFont(uint64_t* font_file);
		MonoAsciiFont *getFont(int c);
	private:
		MonoAsciiFont *font[font_num*sizeof(MonoAsciiFont)];
};
