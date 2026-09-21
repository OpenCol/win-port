/* set_current_nation -- shipped at 1048:4047.
 *
 * Ported by tools/port.py from matched/game/set_current_nation.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 316 bytes at SEG20:0xc2f4 */




void set_current_nation(col_i16 n)
{
    g_cur_nation = n;
    g_cur_nation_rec = &g_nations[g_cur_nation];
}
