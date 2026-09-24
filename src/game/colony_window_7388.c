/* colony_window_7388 -- shipped at 1010:7369.
 *
 * Ported by tools/port.py from matched/game/colony_window_7388.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void colony_buy_build(void *w, col_i16 a);

void colony_window_7388(col_i16 a)
{
    colony_buy_build(&(*g_colony_window), a);
}
