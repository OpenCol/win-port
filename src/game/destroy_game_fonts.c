/* destroy_game_fonts -- shipped at 1050:9873.
 *
 * Ported by tools/port.py from matched/game/destroy_game_fonts.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_font12[];                              DGROUP:0x24ec
 *   extern char g_font16[];                              DGROUP:0x24e8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void Font_dtor_1050_7ff9(void *p, col_i16 flags);

/* same object: 4-byte `push cs; call ` */


void destroy_game_fonts(void)
{
    Font_dtor_1050_7ff9(g_font12, 2);
    Font_dtor_1050_7ff9(g_font16, 2);
}
