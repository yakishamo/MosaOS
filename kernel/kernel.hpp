#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

typedef struct pix_format {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t _rsvd;
} pix_format;

__attribute__((no_caller_saved_registers))
void printd(const char *line, ...);

void print(const char *line, ...);

#endif
