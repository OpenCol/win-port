/* wipe_rows: candidate for 1068:c21f (512 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: wipe_rows
 *
 * THE SHUFFLED-ROW WIPE, the transition behind `1068:c1e9` (the highlighter the woodcut and the congress
 * hall use), `map_draw_tile_rect` and `1010:a28a`.
 *
 * One `new int[w + h]` holds two tables: the first `h` entries are the rows, filled 0..h-1 and then
 * SHUFFLED by h random swaps, and the `w` entries after them are the columns, filled 0..w-1 and then
 * OVERWRITTEN with random values in 0..h-1 -- so the column table is not a permutation at all, and the
 * fill before it is dead.
 *
 * The blit at `1090:0000` then draws `step` rows at a time in that shuffled order, the rectangle is
 * copied to the screen after each pass, and an optional delay in ticks slows it down.
 *
 * The two window arguments both give pixels and bits, but it is the SECOND that is copied to the screen.
 * The blitter's six pointer arguments are pushed as 32-bit values, which is how its prototype takes them
 * (`long`, not pointers).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, GameWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void *port_row_offsets(GRPort *port);
extern "C" void *port_surface_1068_dd01(GRPort *port);
extern "C" void copy_rect_to_screen_1068_d86d(GameWin *w, RECT *r);

void wipe_rows(GameWin *win_a, GameWin *win_b, RECT *src_rect, int x, int y,
                             int mode, int step, int delay)
{
    int w;
    int h;
    int a;
    int b;
    int t;
    /* PORT: i16, Borland's int: asm_wipe_rows reads both tables as words (W4) */
    i16 *cols;
    i16 *tbl;
    RECT r;
    RECT area;
    void *src_rows;
    void *dst_rows;
    register int i;
    register int j;

    r = *src_rect;
    w = rect_width_1068_d79c(src_rect);
    h = rect_height_1068_d7bb(src_rect);
    SetRect(&area, x, y, x + w, y + h);
    src_rows = port_row_offsets((GRPort *)win_a);
    dst_rows = port_row_offsets((GRPort *)win_b);
    tbl = new i16[w + h];
    cols = tbl + h;
    for (j = 0; j < w; j++)
        cols[j] = j;
    for (i = 0; i < h; i++)
        tbl[i] = i;
    for (j = 0; j < w; j++)
        cols[j] = rand_scaled(h);
    for (i = 0; i < h; i++) {
        a = rand_scaled(h);
        b = rand_scaled(h);
        t = tbl[a];
        tbl[a] = tbl[b];
        tbl[b] = t;
    }
    for (i = 0; i < h; i += step) {
        asm_wipe_rows((long)port_surface_1068_dd01((GRPort *)win_a), (long)port_surface_1068_dd01((GRPort *)win_b), (long)cols,
                      (long)tbl, (long)src_rows, (long)dst_rows,
                      i, w, h, step, r.left, r.top, x, y, mode);
        copy_rect_to_screen_1068_d86d(win_b, &area);
        if (delay != 0)
            MSWaitTicks_1068_d93a(delay);
    }
    copy_rect_to_screen_1068_d86d(win_b, &area);
    delete tbl;
}

