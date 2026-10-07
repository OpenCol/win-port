/* music_set_next_group -- shipped at 1040:894a.
 *
 * Ported by tools/port.py from matched/game/music_set_next_group.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_next_group;                             DGROUP:0x1d14
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void music_set_next_group(col_i16 group)
{
    g_next_group = group;
}
