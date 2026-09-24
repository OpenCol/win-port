/* wing_dissolve -- shipped at 1070:cafe.
 *
 * Ported by tools/port.py from matched/platform/wing_dissolve.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_palette_mode;                       DGROUP:0x3380
 *
 * The file's own Surface, Screen dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rand_scaled_1070_ddfe(col_i16 n);
col_i16 rect_height(RECT *r);
col_i16 rect_width(RECT *r);
unsigned char * return_null(Surface *w);
unsigned char * wing_dib_pointer(Surface *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal WinGBitBlt(col_u16 dst, col_i16 dx, col_i16 dy, col_i16 w, col_i16 h, col_u16 src, col_i16 sx, col_i16 sy);
extern col_u16 pascal RealizePalette(col_u16 hdc);
#endif

extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);



/* same object: 4-byte `push cs; call ` */






void wing_dissolve(Surface *dst, Surface *src, Screen *scr, RECT *r, col_i16 stride)
{
    unsigned char *from;
    unsigned char *to;
    unsigned char *src_bits;
    unsigned char *dst_bits;
    col_i16 hcols;
    col_i16 hrows;
    col_i16 *cols;
    col_i16 *rows;
    col_i16 width;
    col_i16 height;
    col_i16 a;
    col_i16 b;
    col_i16 off;
    col_i16 dx;
    col_i16 dy;
    col_i16 sx;
    col_i16 sy;
    register col_i16 i;
    register col_i16 j;

    dx = sx = r->left;
    dy = sy = r->top;
    width = rect_width(r);
    height = rect_height(r);
    hcols = MSAllocateHandle((col_u16)(width * 2));
    hrows = MSAllocateHandle((col_u16)(height * 2));
    src_bits = wing_dib_pointer(src);
    dst_bits = wing_dib_pointer(dst);
    if (scr->palette != 0 && g_palette_mode == 1)
        RealizePalette(scr->hdc);
    cols = (col_i16 *)MSHLock(hcols);
    rows = (col_i16 *)MSHLock(hrows);
    for (i = 0; i < width; i++)
        cols[i] = i;
    for (i = 0; i < height; i++)
        rows[i] = i;
    for (i = 0; i < width; i++) {
        a = rand_scaled_1070_ddfe(width);
        b = rand_scaled_1070_ddfe(width);
        j = cols[a];
        cols[a] = cols[b];
        cols[b] = j;
    }
    for (i = 0; i < height; i++) {
        a = rand_scaled_1070_ddfe(height);
        b = rand_scaled_1070_ddfe(height);
        j = rows[a];
        rows[a] = rows[b];
        rows[b] = j;
    }
    if (width >= height) {
        for (i = 0; i < width; i++) {
            b = 0;
            for (j = 0; j < height; j++) {
                if (j + i < 640) {
                    a = j + i;
                } else {
                    a = b;
                    b++;
                }
                off = cols[a] + rows[j] * stride;
                from = src_bits + (col_u16)off;
                to = dst_bits + (col_u16)off;
                *to = *from;
            }
            WinGBitBlt(scr->hdc, dx, dy, width, height, dst->hdc, sx, sy);
        }
    }
    cols = (col_i16 *)MSHUnlock(hcols);
    rows = (col_i16 *)MSHUnlock(hrows);
    src_bits = return_null(src);
    dst_bits = return_null(dst);
    MSDisposeHandle(hcols);
    MSDisposeHandle(hrows);
}
