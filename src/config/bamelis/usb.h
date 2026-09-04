/*
 * Bamelis fork configuration (usb.h overrides)
 *
 * Applied after config/usb.h via NAMED_CONFIG.  No include guard
 * (see config/bamelis/general.h).
 */

/* Keep USB_KEYBOARD enabled under EFI: iPXE's native xHCI driver takes
 * the USB controller from the firmware to drive USB NICs (e.g. an
 * RTL8153 CDC-ECM adapter), which also disables the firmware keyboard,
 * so iPXE must drive USB keyboards itself.
 */
#undef  USB_KEYBOARD
#define USB_KEYBOARD
