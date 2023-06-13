#include "MosaLoader.h"
#include "elf.h"
#include <Library/BaseMemoryLib.h>
#include <Uefi.h>


void stop() {
	while(1) __asm__("hlt");
}

uint64_t *update_start_addr(uint64_t *start_addr) {
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)start_addr;
	Elf64_Shdr *section_header = (Elf64_Shdr *)((char *)elf_header + 
			elf_header->e_shoff);
	uint64_t updated = (uint64_t)((uintptr_t)start_addr + (uintptr_t)section_header[3].sh_offset);
	return (uint64_t*)updated;
}

EFI_STATUS
EFIAPI
UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	struct bootinfo_t bootinfo;
	EFI_STATUS status;

	Print(L"[log]Start MosaLoader.\n");
	//get gop
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
	status = gBS->LocateProtocol(&gop_guid, NULL, (void **)&gop);
	if(EFI_ERROR(status)) {
		Print(L"[error]LocateProtocol for gop failed.");
		stop();
	}

	//get info from gop
	bootinfo.vinfo.fb = (uint64_t*)gop->Mode->FrameBufferBase;
	bootinfo.vinfo.fb_size = (uint64_t)gop->Mode->FrameBufferSize;
	bootinfo.vinfo.x_axis = (uint32_t)gop->Mode->Info->HorizontalResolution;
	bootinfo.vinfo.y_axis = (uint32_t)gop->Mode->Info->VerticalResolution;
	bootinfo.vinfo.ppsl = (uint32_t)gop->Mode->Info->PixelsPerScanLine;

	//get sfsp
	EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp = NULL;
	status = gBS->LocateProtocol(&sfsp_guid, NULL, (void **)&sfsp);
	if(EFI_ERROR(status)) {
		Print(L"[error]LocateProtocol for sfsp failed.");
		stop();
	}

	//open volume
	EFI_FILE_PROTOCOL *root = NULL;
	sfsp->OpenVolume(sfsp, &root);
	if(EFI_ERROR(status)) {
		Print(L"[error]sfsp->OpenVolume failed.");
		stop();
	}

	//open kernel
	EFI_FILE_PROTOCOL *kernel_file;
	CHAR16 *file_name = (CHAR16*)KERNEL_FILE_NAME;
	UINT64 file_mode = (UINT64)EFI_FILE_READ_ONLY;
	status = root->Open(root, &kernel_file, file_name, file_mode, 0);
	if(EFI_ERROR(status)) {
		Print(L"[error]open kernel file failed.");
		stop();
	}

	//get kernel file info
	EFI_FILE_INFO file_info;
	EFI_GUID fi_guid = EFI_FILE_INFO_ID; 
	UINTN buf_size = BUF_256B;
	status = kernel_file->GetInfo(kernel_file, &fi_guid, &buf_size, &file_info);
	if(EFI_ERROR(status)) {
		Print(L"[error]getting kernel file info failed.");
		stop();
	}
	UINTN kernel_file_size = file_info.FileSize;

	uint64_t *kernel_program = NULL;
	status = gBS->AllocatePool(EfiBootServicesData, kernel_file_size, (void **)&kernel_program);
	status = kernel_file->Read(kernel_file, &kernel_file_size, kernel_program);
	if(EFI_ERROR(status)) {
		Print(L"[error]read kernel failed.");
		stop();
	}

	uint64_t *start_addr = KERNEL_START_QEMU;
	status = gBS->AllocatePages(AllocateAddress, EfiBootServicesData,
			(kernel_file_size+0xfff)/0x1000,
			(EFI_PHYSICAL_ADDRESS*)&start_addr);
	if(EFI_ERROR(status)) {
		Print(L"[ERROR] %d:failed AllocatePool for kernel.\n",__LINE__);
		stop();
	}

	Print(L"[LOG] start_addr : %lx\n", start_addr);
	Print(L"[LOG] kernel_program : %lx\n", kernel_program);
	gBS->CopyMem(start_addr, kernel_program, kernel_file_size);
	Print(L"[LOG] kernel_file_size : %d\n", kernel_file_size);
	Print(L"[LOG] KERNEL_START_QEMU : %lx\n", KERNEL_START_QEMU);
	Print(L"[LOG] start_addr : %lx\n", start_addr);
	uint64_t *updated_start_addr = update_start_addr(start_addr);
	Print(L"[LOG] updated_start_addr : %lx\n", updated_start_addr);
	Print(L"[LOG] *updated_start_addr : %lx\n", *updated_start_addr);
	
	Print(L"exit uefi\n");
	//exit uefi
	UINTN mmapsize = 0, mapkey, descsize;
	UINT32 descver;
	EFI_MEMORY_DESCRIPTOR *mmap = NULL;
	
	do {
		Print(L"[LOG] mmapsize :  %lx\n", mmapsize);
		status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey, &descsize, &descver);
		if(EFI_ERROR(status)) {
			if(mmap) {
				gBS->FreePool(mmap);
			}
			mmapsize += 0x1000;
			do {
				status = gBS->AllocatePool(EfiLoaderData, mmapsize, (void **)&mmap);
				if(EFI_ERROR(status)) {
					Print(L"[ERROR] AllocatePool failed.\n");
					Print(L"[ERROR] mmapsize : %lx\n", mmapsize);
				}
			} while(EFI_ERROR(status));
		} else continue;
	} while(EFI_ERROR(status));

	gBS->ExitBootServices(ImageHandle, mapkey);
	if(EFI_ERROR(status)) {
		Print(L"[ERROR]exit boot service failed.\n");
		Print(L"[ERROR]status = %lx\n", status);
		stop();
	}

	Print(L"[log]jump_to_kernel\n");
	jump_to_kernel(&bootinfo, updated_start_addr);
	return status; //jump_to_kernelから処理は返ってこないので意味はないが
								 //return文がないとコンパイルエラーが出るため記述
}
