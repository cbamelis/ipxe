#!/bin/bash -xe
WANTED_IMGS_BIOS="bin/ipxe.kpxe bin/ipxe.lkrn bin/ipxe.iso bin/ipxe.usb bin/ipxe.dsk bin/undionly.kpxe"
WANTED_IMGS_UEFI="bin-x86_64-efi/ipxe.iso bin-x86_64-efi/ipxe.usb bin-x86_64-efi/ipxe.efi bin-x86_64-efi/snponly.efi"
WANTED_EMBEDS="autoboot_retry bamelis_be"

#WANTED_IMGS="bin/ipxe.kpxe bin/ipxe.usb bin-x86_64-efi/ipxe.efi"
#WANTED_EMBEDS="choose"

DEST=../build

# restore swapped config headers even on interrupt/failure
trap 'git -C src checkout config/console.h config/general.h 2>/dev/null || true' EXIT

# prepare
pushd src
test -d ${DEST} && rm -rf ${DEST}
make clean

# BIOS builds
cp ./config/console.h.bios ./config/console.h
cp ./config/general.h.bios ./config/general.h
mkdir -p ${DEST}/bios
for IMG in ${WANTED_IMGS_BIOS}; do
  IMG=${IMG}
  make ${IMG} && mv ${IMG} ${DEST}/bios || rm ${IMG}
  for CUSTOM in ${WANTED_EMBEDS}; do
    mkdir -p ${DEST}/bios/${CUSTOM}
    make ${IMG} EMBED=../${CUSTOM}.ipxe && mv ${IMG} ${DEST}/bios/${CUSTOM} || rm ${IMG}
  done
done
git checkout ./config/console.h
git checkout ./config/general.h

# UEFI builds
cp ./config/console.h.uefi ./config/console.h
cp ./config/general.h.uefi ./config/general.h
mkdir -p ${DEST}/uefi
for IMG in ${WANTED_IMGS_UEFI}; do
  IMG=${IMG}
  make ${IMG} && mv ${IMG} ${DEST}/uefi || rm ${IMG}
  for CUSTOM in ${WANTED_EMBEDS}; do
    mkdir -p ${DEST}/uefi/${CUSTOM}
    make ${IMG} EMBED=../${CUSTOM}.ipxe && mv ${IMG} ${DEST}/uefi/${CUSTOM} || rm ${IMG}
  done
done
git checkout ./config/console.h
git checkout ./config/general.h

# make UEFI bootable disk image
MNT=../mnt
IMA=${DEST}/uefi/ipxe.dsk
EFI=${DEST}/uefi/ipxe.efi
test -d ${MNT} && rm -rf ${MNT}
mkdir -p ${MNT}
# size the FAT image to the payload (+256 KiB slack), min 1440 KiB
SZ_KB=$(( $(stat -c%s ${EFI}) / 1024 + 256 ))
if [ ${SZ_KB} -lt 1440 ]; then SZ_KB=1440; fi
rm -f ${IMA}
dd if=/dev/zero of=${IMA} bs=1024 count=${SZ_KB}
mkfs.vfat ${IMA}
sudo mount ${IMA} ${MNT}
# UEFI spec removable-media default loader path (bootable without a shell)
sudo mkdir -p ${MNT}/EFI/BOOT
sudo cp ${EFI} ${MNT}/EFI/BOOT/BOOTX64.EFI
# UEFI Shell startup fallback
echo -e "FS0:\ncd EFI\ncd BOOT\nBOOTX64.EFI\n" | sudo tee ${MNT}/startup.nsh
find ${MNT}
sudo umount ${MNT}

popd
