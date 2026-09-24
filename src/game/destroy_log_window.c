/* destroy_log_window -- shipped at 1060:b13b.
 *
 * Ported by tools/port.py from matched/game/destroy_log_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_log_font;                              DGROUP:0x4e68
 *   extern char g_error_ctx;                             DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void Font_dtor_1060_a8ed(void *p, col_i16 flags);
void LogWindow_dtor(void *p, col_i16 flags);

/* same object: 4-byte `push cs; call ` */



void destroy_log_window(void)
{
    Font_dtor_1060_a8ed(&(*g_log_font), 2);
    LogWindow_dtor(&(*g_error_ctx), 2);
}
