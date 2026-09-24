/* map_alloc -- shipped at 1038:d1bd.
 *
 * Ported by tools/port.py from matched/game/map_alloc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_width;                              DGROUP:0x4c40
 *   extern int g_map_h;                                  DGROUP:0x4c42
 *   extern long g_map_cells;                             DGROUP:0x4c3c
 *   extern void far *g_plane0, far *g_plane1, far *g_plane2; DGROUP:0x1c72, DGROUP:0x1c76, DGROUP:0x1c7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 return_zero_1038_cf6e(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void * memset_rtl(void *p, col_i16 c, col_u16 n);

col_i16 map_alloc(col_i16 w, col_i16 h)
{
    col_i16 ww = w;
    col_i16 hh = h;
    col_i16 err = 1;

    (*g_map_width) = ww;
    g_map_h = hh;
    g_map_cells = (col_i32)ww * hh;
    if (return_zero_1038_cf6e())
        goto done;
    memset_rtl(g_plane0, 0x19, (col_u16)g_map_cells);
    memset_rtl(g_plane1, 0, (col_u16)g_map_cells);
    memset_rtl(g_plane2, 0, (col_u16)g_map_cells);
    err = 0;
done:
    return err;
}
