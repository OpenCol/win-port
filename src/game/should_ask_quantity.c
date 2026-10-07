/* should_ask_quantity -- shipped at 1040:7cd7.
 *
 * Ported by tools/port.py from matched/game/should_ask_quantity.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_game_record;                       SEG20:0x05ca
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 pump_keystate_100(void *unused1);

col_i16 should_ask_quantity(void)
{
    return pump_keystate_100(&g_game_record);
}
