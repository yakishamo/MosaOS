extern kernel_main_stack
extern KernelMain

global entry_point
entry_point:
	mov rsp, kernel_main_stack + 1024 * 1024
	call KernelMain
.fin:
	hlt
	jmp .fin
