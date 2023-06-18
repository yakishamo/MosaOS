#include "screen.hpp"
#include "kernel.hpp"
#include "bmp.hpp"

BitMapImage::BitMapImage(uintptr_t file) {
	head = reinterpret_cast<BMHeader*>(file);
	image = reinterpret_cast<uint8_t*>(head->bfOffBits);
	info = reinterpret_cast<BMInfo*>(file + sizeof(BMHeader));
	width = info->bcWidth;
	if(info->bcHeight < 0) {
		height = info->bcHeight * (-1);
		isAscend = true;
	} else {
		height = info->bcHeight;
		isAscend = false;
	}
}

Color_t BitMapImage::getColor(int x, int y) {
	int index = 0;
	if(isAscend) {
		index = width*(height-y-1)+x;
		index *= 4;
	} else {
		index = width*y+x+1;
		index *= 4;
	}
	Color_t c = {image[index+2], image[index+1], image[index]};
	return c;
}

uint32_t BitMapImage::getHeight() {
	return height;
}

uint32_t BitMapImage::getWidth() {
	return width;
}
