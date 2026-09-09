/*
 * Bamelis fork configuration (general.h overrides)
 *
 * Applied after config/general.h via NAMED_CONFIG; build with
 * "make CONFIG=bamelis ...".  Options are forced to the desired end
 * state with #undef + #define so the upstream default does not matter.
 *
 * No include guard: iPXE includes config headers more than once when
 * generating the build fingerprint, so a guard would break it.
 */

/* Download protocols */
#undef  DOWNLOAD_PROTO_HTTPS
#define DOWNLOAD_PROTO_HTTPS	/* Secure Hypertext Transfer Protocol */

/* Commands */
#undef  NTP_CMD
#define NTP_CMD			/* NTP time synchronisation */
#undef  PING_CMD
#define PING_CMD		/* Ping command */
#undef  NUM_CMD
#define NUM_CMD			/* Numeric comparison commands (numlt/numeq/numgt) */

/* Image formats: MultiBoot/PXE/bzImage/COMBOOT on BIOS, EFI on UEFI */
#undef IMAGE_MULTIBOOT
#undef IMAGE_PXE
#undef IMAGE_BZIMAGE
#undef IMAGE_COMBOOT
#undef IMAGE_EFI
#if defined ( PLATFORM_efi )
  #define IMAGE_EFI
#else
  #define IMAGE_MULTIBOOT
  #define IMAGE_PXE
  #define IMAGE_BZIMAGE
  #define IMAGE_COMBOOT
#endif
