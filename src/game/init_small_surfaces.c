/* init_small_surfaces -- shipped at 1040:2126.
 *
 * Ported by tools/port.py from matched/game/init_small_surfaces.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_flag_sprite[];                     SEG28:0x0046
 *   extern char far g_port_66[];                         SEG28:0x0066
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_make_solid(void *p, col_i16 a, col_i16 b, col_i16 c);
extern void GRPort_Init_wh(void *p, col_i16 w, col_i16 h);

void init_small_surfaces(void)
{
    sprite_make_solid(g_flag_sprite, 12, 13, 0x31);
    GRPort_Init_wh(g_port_66, 32, 32);
}
