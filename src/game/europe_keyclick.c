/* europe_keyclick -- shipped at 1048:3d21.
 *
 * Ported by tools/port.py from matched/game/europe_keyclick.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_europe_win_rec;                    SEG20:0x2ff8
 *   extern void far * far g_current_window;              DGROUP:0x2ae0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void godmode_money_keys(col_i16 key);

void europe_keyclick(col_i16 key)
{
    g_current_window = &g_europe_win_rec;
    godmode_money_keys(key);
}
