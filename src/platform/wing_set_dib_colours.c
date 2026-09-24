/* wing_set_dib_colours -- shipped at 1070:a93c.
 *
 * Ported by tools/port.py from matched/platform/wing_set_dib_colours.c.
 *
 * The file's own RGBQUAD, Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hwnd -> hdc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void palette_entry_rgb(void *pal, col_i16 i, void *r, void *g,
                                     void *b);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal WinGSetDIBColorTable(col_u16 hdc, col_u16 start, col_u16 n,
                                           RGBQUAD *t);
#endif

/* same object: 4-byte `push cs; call ` */


void wing_set_dib_colours(Win *w, void *pal, col_i16 start, col_i16 count)
{
    unsigned char tbl[0x400];
    register col_i16 i;

    for (i = start; start + count > i; i++) {
        palette_entry_rgb(pal, i, i * 4 + tbl + 2, i * 4 + tbl + 1, i * 4 + tbl);
        *(i * 4 + tbl + 3) = 0;
    }
    WinGSetDIBColorTable(w->hdc, start, count, (RGBQUAD *)tbl);
}
