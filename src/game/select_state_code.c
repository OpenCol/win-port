/* select_state_code -- shipped at 1038:47ab.
 *
 * Ported by tools/port.py from matched/game/select_state_code.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern struct rec far g_recs[];                      SEG20:0x7841
 *   extern char far g_game_flags;                        SEG20:0x7784
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

struct rec { char busy; char pad[0x33]; };



col_i16 select_state_code(col_i16 x, col_i16 y)
{
    col_i16 r;

    switch (x) {
    case 2:
        r = 0x12;
        break;
    case 3:
        r = 0xb;
        break;
    case 1:
        r = (y < 4 && g_recs[y].busy == 0) ? (((*g_game_flags) & 1) ? 7 : 4) : 8;
        break;
    case 0:
    default:
        r = (y < 4 && g_recs[y].busy == 0) ? (((*g_game_flags) & 1) ? 9 : 4) : 6;
        break;
    }
    return r;
}
