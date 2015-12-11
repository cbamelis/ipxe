#!/bin/bash
WANTED_IMGS="ipxe.kpxe ipxe.lkrn ipxe.usb undionly.kpxe"
WANTED_EMBEDS="bamelis_be tomtom_mnr01"
TO_EMBED=../${CUSTOM}.ipxe
DEST=../build

pushd src
test -d ${DEST} && rm -rf ${DEST}
test -d ${DEST}/${CUSTOM} || mkdir -p ${DEST}/${CUSTOM}
make clean
for IMG in ${WANTED_IMGS}; do
  IMG=bin/${IMG}
  make ${IMG} && mv ${IMG} ${DEST} || rm ${IMG}
  for CUSTOM in ${WANTED_EMBEDS}; do
    test -d ${DEST}/${CUSTOM} || mkdir -p ${DEST}/${CUSTOM}
    make ${IMG} EMBED=../${CUSTOM}.ipxe && mv ${IMG} ${DEST}/${CUSTOM} || rm ${IMG}
  done
done
popd
