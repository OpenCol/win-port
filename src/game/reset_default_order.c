/* reset_default_order -- shipped at 1038:4f6d.
 *
 * Ported by tools/port.py from matched/game/reset_default_order.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_nation_colour;            DGROUP:0x009a
 *   extern unsigned char far g_order_9b;                 DGROUP:0x009b
 *   extern unsigned char far g_order_9c;                 DGROUP:0x009c
 *   extern unsigned char far g_order_9d;                 DGROUP:0x009d
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void reset_default_order(void)
{
    (*g_nation_colour) = 10;
    g_order_9b = 11;
    g_order_9c = 12;
    g_order_9d = 13;
}
