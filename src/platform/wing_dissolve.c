/* wing_dissolve: candidate for 1070:cafe (849 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * A DISSOLVE from one WinG surface to another over rectangle `r`, blitting to the screen as it goes.
 * No direct caller was found (the knowledge base lists none).
 *
 * Two scratch permutations are allocated: the rectangle's columns and its rows, each filled with
 * 0..n-1 and shuffled by n random swaps (`rand_scaled_1070_ddfe`). The palette is realised first when
 * the screen has one (+0xa) and the palette mode at DGROUP:0x3380 is 1.
 *
 * THE SWEEP runs only when the rectangle is at least as wide as it is tall. For each step i across the
 * width, row j takes the column at permutation index i + j. Past 639 that index comes instead from a
 * second counter starting at 0, so the table is assumed to be 640 wide. The pixel at
 * `cols[k] + rows[j] * stride` is copied from `src` into `dst`, and `dst` is blitted to the screen DC
 * (+4) after every column step. A taller-than-wide rectangle is not drawn at all.
 *
 * The offset is added to the `` bit pointers as an UNSIGNED 16-bit value (`XOR DX,DX`), so the
 * copy never reaches past the first 64K of either image. The column table is `` and the row table
 * plain ``, as the bytes show (only one gets the `__AHSHIFT` walk). The swap temporary and the
 * sweep's inner counter share one register variable. It compiled RELOCATABLE on the first build.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern int rect_width(RECT *r);
extern int rect_height(RECT *r);
extern unsigned char *wing_dib_pointer(Surface *graf);

void wing_dissolve(Surface *dst, Surface *src, MSWin *scr, RECT *r, int stride)
{
    unsigned char *from;
    unsigned char *to;
    unsigned char *src_bits;
    unsigned char *dst_bits;
    short hcols;
    short hrows;
    int *cols;
    int *rows;
    int width;
    int height;
    int a;
    int b;
    int off;
    int dx;
    int dy;
    int sx;
    int sy;
    register int i;
    register int j;

    dx = sx = r->left;
    dy = sy = r->top;
    width = rect_width(r);
    height = rect_height(r);
    hcols = MSAllocateHandle((unsigned)(width * 2));
    hrows = MSAllocateHandle((unsigned)(height * 2));
    src_bits = wing_dib_pointer((GrafRec *)src);
    dst_bits = wing_dib_pointer((GrafRec *)dst);
    if (scr->hpal != 0 && g_palette_mode == 1)
        RealizePalette(scr->hdc);
    cols = (int *)MSHLock(hcols);
    rows = (int *)MSHLock(hrows);
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
                from = src_bits + (unsigned)off;
                to = dst_bits + (unsigned)off;
                *to = *from;
            }
            WinGBitBlt(scr->hdc, dx, dy, width, height, dst->hdc, sx, sy);
        }
    }
    cols = (int *)MSHUnlock(hcols);
    rows = (int *)MSHUnlock(hrows);
    src_bits = return_null(src);
    dst_bits = return_null(dst);
    MSDisposeHandle(hcols);
    MSDisposeHandle(hrows);
}
