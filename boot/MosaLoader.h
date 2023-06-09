#ifndef BROTLOADER_H
#define BROTLOADER_H

#include <stdint.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

#include "boot_types.h"

extern void jump_to_kernel(bootinfo_t *binfo, uint64_t *kernel_addr);

#define KERNEL_FILE_NAME L"kernel.elf"
#define FONT_FILE_NAME L"font.bin"
#define BUF_256B (UINTN)256
#define KERNEL_START_QEMU (uint64_t*)0x100000

#endif /* BROTLOADER_H */

