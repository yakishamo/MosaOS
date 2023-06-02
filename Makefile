run:
	cp ~/edk2/Build/MosaLoaderPkgX64/DEBUG_CLANG38/X64/MosaLoader.efi ./fs/EFI/BOOT/BOOTX64.efi
	cp kernel/kernel.elf ./fs/
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,readonly=on,file=tool/OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=tool/OVMF_VARS.fd \
		fat:rw:fs/ -m 4G \
		-display spice-app \
		-monitor stdio
