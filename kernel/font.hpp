#pragma once
#include <stdint.h>

const int row_num = 16;
const int font_num = 128;

class MonoAsciiFont {
	public:
		MonoAsciiFont();		
		bool isDraw(unsigned int row, unsigned int column);

	private:
		uint8_t row[row_num];
};

class AsciiFont {
	public:
		AsciiFont(); 
	private:
		MonoAsciiFont **font;
};
