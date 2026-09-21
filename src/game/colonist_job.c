/* colonist_job -- shipped at 1018:4c11.
 *
 * Ported by tools/port.py from matched/game/colonist_job.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 nth_colonist_unit(col_i16 n);
col_i16 unit_colonist_class(col_i16 u);

/* 0xca bytes */






col_i16 colonist_job(col_i16 c)
{
    col_i16 r;

    if (g_cur_colony_rec->pop > c)
        r = ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[c];
    else
        r = unit_colonist_class(nth_colonist_unit(c - g_cur_colony_rec->pop));
    return r;
}
