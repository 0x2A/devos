#!/bin/bash

#dd bs=512 count=2880 if=/dev/zero of=build/osdev.img
#imdisk -a -f "build/osdev.img" -s 1440K -m B: #mount img to virtual drive and format it with windows cmd since cygwin dont have mkfs
#cmd.exe /c format B: #format using windows format command since cygwin dont have mkfs >:(

mkdir -p build/isodir
mkdir -p build/isodir/boot/
cp build/kernel.bin build/isodir/boot/devos.bin
mkdir -p build/isodir/boot/grub
cp config/grub.cfg build/isodir/boot/grub/grub.cfg
grub-mkrescue --output=build/devos.iso build/isodir
rm -R build/isodir
