/* autosave_to_slot -- shipped at 1008:0000.
 *
 * Ported by tools/port.py from matched/game/autosave_to_slot.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_year;                               SEG20:0x778c
 *   extern int far g_season;                             SEG20:0x778e
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void save_game_to_slot(col_i16 n);

void autosave_to_slot(void)
{
    if (g_year % 10 == 0 && g_season == 0 && g_turn_counter > 2)
        save_game_to_slot(8);
    else
        save_game_to_slot(9);
}
