run:
	sudo mount disk.img ./mnt
	sudo cp ~/edk2/Build/MosaLoaderPkgX64/DEBUG_CLANG38/X64/MosaLoader.efi .
	sudo cp ./MosaLoader.efi ./mnt/EFI/BOOT/BOOTX64.efi
	sudo cp kernel/kernel.elf .
	sudo cp ./kernel.elf ./mnt/
	sudo umount mnt
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,readonly=on,file=./tool/OVMF_CODE.fd \
		-drive if=pflash,format=raw,file=./tool/OVMF_VARS.fd \
		-drive if=ide,index=0,media=disk,format=raw,file=./disk.img \
		-device nec-usb-xhci,id=xhci \
		-m 4G \
		-display spice-app \
		-monitor stdio \
		-gdb tcp::1234
