/* set_current_tribe -- shipped at 1038:0000.
 *
 * Ported by tools/port.py from matched/game/set_current_tribe.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Tribe far g_tribes[8];                        SEG20:0xd182
 *   extern int far g_cur_tribe;                          DGROUP:0x4c30
 *   extern int far g_cur_tribe_nation;                   DGROUP:0x4c32
 *   extern Tribe far * far g_cur_tribe_rec;              DGROUP:0x4c28
 *
 * The file's own Tribe dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_current_tribe(col_i16 t)
{
    g_cur_tribe = t;
    if (t < 0 || t >= 8)
        t = 0;
    g_cur_tribe_nation = t + 4;
    g_cur_tribe_rec = &g_tribes[t];
}
