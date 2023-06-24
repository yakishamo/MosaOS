#include "interrupt.hpp"
#include "screen.hpp"
#include "asmfunc.hpp"
#include "kernel.hpp"
#include "x86_descriptor.hpp"

InterruptDescriptor idt[256];

void SetIDTEntry(InterruptDescriptor& desc,
		InterruptDescriptorAttribute attr,
		uint64_t offset,
		uint16_t segment_selector) {
	desc.attr = attr;
	desc.offset_low = offset & 0xffffu;
	desc.offset_middle = (offset >> 16) & 0xffffu;
	desc.offset_high = offset >> 32;
	desc.segment_selector = segment_selector;
}

__attribute__((no_caller_saved_registers))
void NotifyEndOfInterrupt() {
	volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);
	*end_of_interrupt = 0;
}

__attribute__((interrupt))
void IntSegFault(InterruptFrame *frame) {
	printd("Segmentation Fault");
	while(1)__asm__("hlt");
	NotifyEndOfInterrupt();
}

__attribute__((interrupt))
void IntDefault(InterruptFrame *frame) {
	printd("some Interrupt");
	while(1) __asm__("hlt");
	NotifyEndOfInterrupt();
}

void InitializeInterrupt() {
	const uint16_t cs = GetCS();
	for(int i = 0; i < 0x14; i++) {
		SetIDTEntry(idt[i], MakeIDTAttr(DescriptorType::kInterruptGate, 0),
				reinterpret_cast<uint64_t>(IntDefault), cs);
	}
	SetIDTEntry(idt[0x0c], MakeIDTAttr(DescriptorType::kInterruptGate, 0),
			reinterpret_cast<uint64_t>(IntSegFault), cs);
	LoadIDT(sizeof(idt)-1, reinterpret_cast<uintptr_t>(&idt[0]));
}
