/* nation_text_field2 -- shipped at 1030:80f2.
 *
 * Ported by tools/port.py from matched/game/nation_text_field2.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern TribeName far g_tribe[];                      SEG20:0xd3f2
 *   extern int far g_nation_adjective[];                 SEG20:0xd89e
 *
 * The file's own TribeName dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 6 bytes at SEG20:0xd3f2;
                    the TRIBES resource is "Incas, Inca, Jewelled Relics", so +2 is the
                    ADJECTIVE -- which is why the European branch falls back to the
                    adjective table */



col_i16 nation_text_field2(col_i16 n)
{
    if (n >= 4)
        return (g_tribe - 4)[n].adjective;
    else
        return g_nation_adjective[n];
}
