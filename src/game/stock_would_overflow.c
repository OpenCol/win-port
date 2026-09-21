/* stock_would_overflow -- shipped at 1018:499d.
 *
 * Ported by tools/port.py from matched/game/stock_would_overflow.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int far g_prod[];                             SEG21:0x0122
 *   extern int far g_value[];                            SEG21:0x0164
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0xca bytes */




col_i16 stock_would_overflow(col_i16 k)
{
    return g_value[k] < g_prod[k] + ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[k];
}
