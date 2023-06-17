TARGET = kernel.elf
CFLAGS += -g -O0 \
					-Wall -Wextra -Wno-unused-parameter \
					-I$(HOME)/x86_64-elf/include \
					-I./ \
					-fno-PIE \
					-fno-builtin -fno-common -fno-stack-protector \
					-nostdlib -nostdinc \
					-mno-red-zone

CPPFLAGS = -g -O0 \
					 -Wall -Wextra -Wno-unused-parameter \
					 -I$(HOME)/x86_64-elf/include/c++/v1 \
					 -I$(HOME)/x86_64-elf/include \
					 -I./ \
					 -nostdinc -nostdlib \
					 -fno-builtin -fno-PIE -ffreestanding \
					 -fno-common -fno-stack-protector -fno-exceptions\
					 -fno-rtti \
					 -mno-red-zone

LDFLAGS = -Map kernel.map -e entry_point -z norelro --image-base 0x100000 -static\
					-L$(HOME)/x86_64-elf/lib/ -lc -lc++

OBJS = kernel.o font.o screen.o new.o asmfunc.o segment.o paging.o

.PHONY: clean

$(TARGET): $(OBJS)
	ld.lld $(LDFLAGS) -o $@ $+

%.o: %.cpp
	clang++ $(CPPFLAGS) -c -o $@ $<

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

clean:
	rm -f *.o *.elf *.map
