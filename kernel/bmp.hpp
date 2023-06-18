#pragma once
#include <cstdint>
#include "screen.hpp"
#include "kernel.hpp"

typedef struct BMHader {
	char bfType[2];
	uint32_t bfSize;
	uint32_t bfReserved;
	uint32_t bfOffBits;
}__attribute__((packed)) BMHeader;

typedef struct BMInfo {
	uint32_t bcSize;
	uint32_t bcWidth;
	int32_t bcHeight;
	uint16_t bcPlanes;
	uint16_t bcBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPixPerMeter;
	uint32_t biYPixPerMeter;
	uint32_t biClrUsed;
	uint32_t biCirImportant;
} __attribute__((packed)) BMInfo;

class BitMapImage {
	public:
		BitMapImage(uintptr_t file);
		Color_t getColor(int x, int y);
		uint32_t getWidth();
		uint32_t getHeight();
	private:
		BMHeader *head;
		BMInfo *info;
		uint8_t *image;
		uint32_t width;
		uint32_t height;
		bool isAscend;
};
