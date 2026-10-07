/* grow_region_rect: candidate for 1068:31a6 (603 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: grow_region_rect
 * size-source: 603, not the census 594. The trailing 9 bytes are the dead third epilogue.
 *
 * Grows one rectangle over a byte grid, starting from the 1x1 rectangle at (x0, y0) and returning
 * it by value. Rows are scanned from y0 down (from x0 on the first row, from 0 after), and every
 * cell still marked 0xFF is a candidate: its 17x17 neighbourhood, clipped to the grid, is
 * compared with the rectangle on each axis, and the cell is absorbed only if the union on that
 * axis is no wider than the two extents added together -- that is, if the neighbourhood touches
 * or overlaps the rectangle. An absorbed cell widens the rectangle to include it and is cleared to
 * 0. Nine consecutive rows that absorb nothing end the scan early (the idle count must exceed 8).
 *
 * The grid is `w` by `h` at +0x2e and +0x30, with `stride` bytes per row at +6 and the cells
 * behind the pointer at +0x18, so the row is `cells + (long)y * stride` through `LXMUL` and
 * `__AHSHIFT`. `build_region_rects` calls it once per 0xFF cell it finds and stores the
 * answer straight into the next slot of its rectangle array.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

RECT grow_region_rect(Flic *grid, int x0, int y0)
{
    unsigned char *row;
    RECT r;
    int yhi;
    int xhi;
    int ylo;
    int xlo;
    int idle;
    int changed;
    int span;
    int gap;
    register int x;
    register int y;

    SetRect(&r, x0, y0, x0 + 1, y0 + 1);
    idle = 0;
    for (y = y0; (unsigned)grid->height > y; y++) {
        row = grid->bits + (long)y * grid->stride;
        changed = 0;
        for (x = y == y0 ? x0 : 0; (unsigned)grid->width > x; x++) {
            if (row[x] == 0xff) {
                ylo = y - 8;
                yhi = y + 9;
                xlo = x - 8;
                xhi = x + 9;
                if (ylo < 0)
                    ylo = 0;
                if ((unsigned)grid->height < yhi)
                    yhi = grid->height;
                if (xlo < 0)
                    xlo = 0;
                if ((unsigned)grid->width < xhi)
                    xhi = grid->width;
                span = (r.right - r.left) + (xhi - xlo);
                gap = (xhi < r.right ? r.right : xhi) - (xlo > r.left ? r.left : xlo);
                if (gap <= span) {
                    span = (r.bottom - r.top) + (yhi - ylo);
                    gap = (yhi < r.bottom ? r.bottom : yhi) - (ylo > r.top ? r.top : ylo);
                    if (gap <= span) {
                        if (x < r.left)
                            r.left = x;
                        else if (x >= r.right)
                            r.right = x + 1;
                        if (y < r.top)
                            r.top = y;
                        else if (y >= r.bottom)
                            r.bottom = y + 1;
                        row[x] = 0;
                        changed = 1;
                    }
                }
            }
        }
        if (changed == 1) {
            idle = 0;
        } else {
            idle++;
            if (idle > 8)
                return r;
        }
    }
    return r;
}
