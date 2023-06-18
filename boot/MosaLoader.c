#include "MosaLoader.h"
#include "memory_map.h"
#include "elf.h"
#include <Library/BaseMemoryLib.h>
#include <Uefi.h>

void stop() {
	while(2) __asm__("hlt");
}

int strcmp(const char *first, const char *second) {
	int i = 0;
	while(first[i] != '\0' && second[i] != '\0') {
		if(first[i] != second[i]) {
			return 1;
		}
		i++;
	}
	if(first[i] == second[i]) {
		return 0;
	} else {
		return 1;
	}
}

void dumpSectionHeader(Elf64_Shdr *sh, int i) {
	Print(L"[LOG] sh_name    : %x\n", sh[i].sh_name);
	Print(L"[LOG] sh_type    : %x\n", sh[i].sh_type);
	Print(L"[LOG] sh_flags   : %x\n", sh[i].sh_flags);
	Print(L"[LOG] sh_addr    : %x\n", sh[i].sh_addr);
	Print(L"[LOG] sh_offset  : %x\n", sh[i].sh_offset);
	Print(L"[LOG] sh_size    : %x\n", sh[i].sh_size);
	Print(L"[LOG] sh_link    : %x\n", sh[i].sh_link);
	Print(L"[LOG] sh_info    : %x\n", sh[i].sh_info);
	Print(L"[LOG] sh_addralign : %x\n", sh[i].sh_addralign);
	Print(L"[LOG] sh_entsize : %x\n", sh[i].sh_entsize);
	return;
}

uintptr_t Allocate(uintptr_t addr, uintptr_t size) {
	UINT64 ret = addr;
	int tmp = 0;
	UINTN num_pages = (size + 0xfff) / 0x1000;

	EFI_STATUS status;
	do {
		tmp++;
		ret = (ret/0x1000)*0x1000;
		status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, num_pages, &ret);
		//Print(L"[LOG] EfiLoaderData : %d\n", EfiLoaderData);
		//Print(L"[LOG] ret : %lx\n", ret);
		//Print(L"[LOG] num_pages : %d\n", num_pages);
		if(EFI_ERROR(status)) {
			//Print(L"[ERROR] status : %lx\n", status);
			//Print(L"[ERROR] %d:Allocate failed.\n", __LINE__);
			//Print(L"[ERROR] addr : 0x%lx\n", addr);
			//Print(L"[ERROR] size : 0x%lx\n", size);
			//Print(L"[ERROR] num_pages = %d\n", num_pages);
			ret += 0x1000;
			size -= 0x1000;
			num_pages = (size+0xfff) / 0x1000;
			if(tmp == 10000){
				Print(L"[ERROR] status : %lx\n", status);
				Print(L"[ERROR] %d:Allocate failed.\n", __LINE__);
				Print(L"[ERROR] addr : 0x%lx\n", ret);
				Print(L"[ERROR] size : 0x%lx\n", size);
				Print(L"[ERROR] num_pages = %d\n", num_pages);
				while(1) __asm__("hlt");
			}
		}
	}while(EFI_ERROR(status) && size < 0xffffffffffff0000);
	return ret;
}

uintptr_t LoadKernel(uintptr_t start_addr) {
	Elf64_Ehdr *eh = (Elf64_Ehdr *)start_addr;
	Elf64_Phdr *pht = (Elf64_Phdr*)((uintptr_t)start_addr + eh->e_phoff);
	uint8_t ph_num = eh->e_phnum;
	uintptr_t ph_size = eh->e_phentsize;
	for(int i = 0; i < ph_num; i++) {
		if(pht->p_type == PT_LOAD) {
			Print(L"[LOG] p_memsz = 0x%lx\n", pht->p_memsz);
			Print(L"[LOG] p_type = %d\n", pht->p_type);
			Print(L"[LOG] i = %d\n", i);
			Allocate(pht->p_vaddr, pht->p_memsz);
			gBS->CopyMem((void*)pht->p_vaddr, 
					(void*)start_addr + pht->p_offset, pht->p_filesz);
		}
		pht = (Elf64_Phdr *)((uintptr_t)pht + ph_size);
	}
	return eh->e_entry;
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
	bootinfo.vinfo.fb = (uint32_t*)gop->Mode->FrameBufferBase;
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

	//open font.bin
	EFI_FILE_PROTOCOL *font_file;
	CHAR16 *font_file_name = (CHAR16*)FONT_FILE_NAME;
	UINT64 file_mode = (UINT64)EFI_FILE_READ_ONLY;
	status = root->Open(root, &font_file, font_file_name, file_mode, 0);
	if(EFI_ERROR(status)) {
		Print(L"[ERROR] %d : open font file failed.\n", __LINE__);
		stop();
	}

	//get font file info
	EFI_FILE_INFO font_file_info;
	EFI_GUID fi_guid = EFI_FILE_INFO_ID;
	UINTN font_buf_size = BUF_256B;
	status = font_file->GetInfo(font_file, &fi_guid, &font_buf_size, &font_file_info);
	if(EFI_ERROR(status)) {
		Print(L"[ERROR] %d : getting font file info failed.\n", __LINE__);
		stop();
	}
	
	UINTN font_file_size = font_file_info.FileSize;
	uint64_t *font_buf = NULL;
	status = gBS->AllocatePool(EfiLoaderData, font_file_size, (void**)&font_buf);
	status = font_file->Read(font_file, &font_file_size, font_buf);
	bootinfo.font = font_buf;

	//open kernel
	EFI_FILE_PROTOCOL *kernel_file;
	CHAR16 *kernel_file_name = (CHAR16*)KERNEL_FILE_NAME;
	status = root->Open(root, &kernel_file, kernel_file_name, file_mode, 0);
	if(EFI_ERROR(status)) {
		Print(L"[error]open kernel file failed.");
		stop();
	}

	//get kernel file info
	EFI_FILE_INFO kernel_file_info;
	UINTN kernel_buf_size = BUF_256B;
	status = kernel_file->GetInfo(kernel_file, &fi_guid, &kernel_buf_size, &kernel_file_info);
	if(EFI_ERROR(status)) {
		Print(L"[error]getting kernel file info failed.");
		stop();
	}
	UINTN kernel_file_size = kernel_file_info.FileSize;

	uint64_t *kernel_program = NULL;
	status = gBS->AllocatePool(EfiLoaderData, kernel_file_size, (void **)&kernel_program);
	status = kernel_file->Read(kernel_file, &kernel_file_size, kernel_program);
	if(EFI_ERROR(status)) {
		Print(L"[error]read kernel failed.");
		stop();
	}

	uint64_t *updated_start_addr = (uint64_t *)LoadKernel((uintptr_t)kernel_program);
	
	Print(L"exit uefi\n");
	//exit uefi
	CHAR8 memmap_buf[4096*4];
	MemoryMap_t mmap={sizeof(memmap_buf), memmap_buf, 0,0,0,0};
	
	status = gBS->GetMemoryMap(&mmap.buffer_size,
			mmap.buffer,
			&mmap.map_key, 
			&mmap.descriptor_size, 
			&mmap.descriptor_version);

	mmap.map_size = mmap.buffer_size;
	status = gBS->ExitBootServices(ImageHandle, mmap.map_key);
	if(EFI_ERROR(status)) {
		Print(L"[ERROR] exit boot service failed.\n");
		Print(L"[ERROR] status = %lx\n", status);
		stop();
	}
	bootinfo.mmap = &mmap;
	jump_to_kernel(&bootinfo, updated_start_addr);
	return status; //jump_to_kernelから処理は返ってこないので意味はないが
								 //return文がないとコンパイルエラーが出るため記述
}
