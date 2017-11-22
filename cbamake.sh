#!/bin/bash -xe
WANTED_IMGS="bin/ipxe.kpxe bin/ipxe.lkrn bin/ipxe.iso bin/ipxe.usb bin/ipxe.dsk bin/undionly.kpxe bin-x86_64-efi/ipxe.efi bin-x86_64-efi/snponly.efi"
WANTED_EMBEDS="bamelis_be choose tomtom_mnr01"

#WANTED_IMGS="bin/ipxe.kpxe bin/ipxe.usb bin-x86_64-efi/ipxe.efi"
#WANTED_EMBEDS="choose"

TO_EMBED=../${CUSTOM}.ipxe
DEST=../build

pushd src
test -d ${DEST} && rm -rf ${DEST}
test -d ${DEST}/${CUSTOM} || mkdir -p ${DEST}/${CUSTOM}
make clean
for IMG in ${WANTED_IMGS}; do
  make ${IMG} && mv ${IMG} ${DEST} || rm ${IMG}
  for CUSTOM in ${WANTED_EMBEDS}; do
    test -d ${DEST}/${CUSTOM} || mkdir -p ${DEST}/${CUSTOM}
    make ${IMG} EMBED=../${CUSTOM}.ipxe && mv ${IMG} ${DEST}/${CUSTOM} || rm ${IMG}
  done
done

# make UEFI bootable floppy with built-in choose menu
MNT=../mnt
IMA=${DEST}/choose/ipxe_virtualbox_efi_bootable_floppy.dsk
test -d ${MNT} && rm -rf ${MNT}
mkdir -p ${MNT}
mkfs.msdos -C ${IMA} 1440
sudo mount ${IMA} ${MNT}
sudo mkdir -p ${MNT}/boot/efi
echo -e "FS0:\ncd boot\ncd efi\nbootx64.efi\n" | sudo tee ${MNT}/startup.nsh
sudo cp ${DEST}/choose/ipxe.efi ${MNT}/boot/efi/bootx64.efi
find ${MNT}
sudo umount ${MNT}

popd
