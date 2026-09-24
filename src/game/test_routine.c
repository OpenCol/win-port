/* test_routine -- shipped at 1048:b95a.
 *
 * Ported by tools/port.py from matched/game/test_routine.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ask_text_resource(char *id);
void dialog_set_option_value(col_i16 slot, col_i16 v);

void test_routine(void)
{
    g_game_flags &= ~0xb;
    dialog_set_option_value(0, g_unit_count);
    dialog_set_option_value(1, g_colony_count);
    ask_text_resource("TEST");
}
