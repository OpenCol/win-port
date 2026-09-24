/* destroy_map_ports -- shipped at 1040:45a5.
 *
 * Ported by tools/port.py from matched/game/destroy_map_ports.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_port_66[];                         SEG28:0x0066
 *   extern char far g_flag_sprite[];                     SEG28:0x0046
 *   extern char far g_sprite_26[];                       SEG28:0x0026
 *   extern char far g_backdrop_port[];                   SEG28:0x0000
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void GRPort_dtor(void *p, col_i16 flags);
extern void Sprite_dtor(void *p, col_i16 flags);

void destroy_map_ports(void)
{
    GRPort_dtor(g_port_66, 2);
    Sprite_dtor(g_flag_sprite, 2);
    Sprite_dtor(g_sprite_26, 2);
    GRPort_dtor(g_backdrop_port, 2);
}
