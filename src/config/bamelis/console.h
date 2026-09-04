/*
 * Bamelis fork configuration (console.h overrides)
 *
 * Applied after config/console.h via NAMED_CONFIG.  No include guard
 * (see config/bamelis/general.h).
 */

/* Graphical framebuffer console on all platforms (for menus), including
 * BIOS, where upstream disables it by default.
 */
#undef  CONSOLE_FRAMEBUFFER
#define CONSOLE_FRAMEBUFFER

/* Runtime-selectable keyboard map (needed for the Belgian layout) on all
 * platforms; upstream defaults to "us" except on EFI.
 */
#undef  KEYBOARD_MAP
#define KEYBOARD_MAP	dynamic
