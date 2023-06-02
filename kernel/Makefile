TARGET = kernel.elf
CFLAGS += -g -O0 \
					-Wall -Wextra -Wno-unused-parameter \
					-I$(HOME)/x86_64-elf/include \
					-fPIE \
					-fno-builtin -fno-common -fno-stack-protector \
					-nostdlib -nostdinc \
					-mno-red-zone

CPPFLAGS = -g -O0 \
					 -Wall -Wextra -Wno-unused-parameter \
					 -I$(HOME)/x86_64-elf/include \
					 -I$(HOME)/x86_64-elf/include/c++/v1 \
					 -nostdinc -nostdlib \
					 -fPIE \
					 -fno-builtin -fno-common -fno-stack-protector \
					 -mno-red-zone

LDFLAGS = -Map kernel.map -e entry_point -z norelro --image-base 0x100000 -static\
					-L$(HOME)/x86_64-elf/lib/ -lc -lc++

OBJS = kernel.o

.PHONY: clean

$(TARGET): $(OBJS)
	ld.lld $(LDFLAGS) -o $@ $+

%.o: %.cpp
	clang++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf *.map
