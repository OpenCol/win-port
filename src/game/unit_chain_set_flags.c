/* unit_chain_set_flags -- shipped at 1038:2b17.
 *
 * Ported by tools/port.py from matched/game/unit_chain_set_flags.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain_next(col_i16 u);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */





void unit_chain_set_flags(col_i16 u, col_i16 f)
{
    while (u >= 0) {
        g_units[u].flags |= f;
        u = unit_chain_next(u);
    }
}
