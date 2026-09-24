/* colony_keyclick -- shipped at 1010:36fe.
 *
 * Ported by tools/port.py from matched/game/colony_keyclick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_win_rec;                    SEG20:0x25e4
 *   extern void far * far g_current_window;              DGROUP:0x2ae0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ColonyWin_KeyClick(col_i16 key);

void colony_keyclick(col_i16 key)
{
    g_current_window = &g_colony_win_rec;
    ColonyWin_KeyClick(key);
}
