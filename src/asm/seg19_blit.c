/* Segment 19's blitters, in C (roadmap W5): asm_wipe_rows (1090:0000), asm_copy_bits (1090:00ff),
 * asm_fill_rect (1090:01b4), asm_draw_sprite (1090:0215), asm_draw_mono_sprite (1090:0348).
 *
 * Written from the shipped routines block by block, so that W5's differential can hold them to
 * the originals. They are 386 code over 32-bit offsets: a bitmap is a base and a signed stride,
 * a stride <= 0 meaning bottom-up (the row counted from the bottom, `height - 1 - y`). Loops
 * are do-while where the original's are (a count of 0 is 65536 passes there; kept). Each
 * parameter is named for the stack slot it is read from.
 */
#include <stdint.h>

/* the row's byte offset: a positive stride counts rows from the top, else from the bottom */
static uint32_t row_at(int16_t stride, uint16_t height, uint16_t y, uint16_t x)
{
    uint32_t s;
    uint16_t row;
    if (stride > 0) {
        s = (uint32_t)stride;
        row = y;
    } else {
        s = (uint32_t)(-(int32_t)stride);
        row = (uint16_t)(height - 1 - y);
    }
    return (uint32_t)row * s + x;
}

/* 1090:00ff: a w x h rectangle from (sx, sy) to (dx, dy), each bitmap with its own height and
   signed stride; after a row each pointer moves by its own stride less w */
void asm_copy_bits(void *src, void *dst, int sx, int sy, int dx, int dy, int w, int h, int src_h, int dst_h,
                   int src_stride, int dst_stride)
{
    unsigned char *s = (unsigned char *)src + row_at((int16_t)src_stride, (uint16_t)src_h, (uint16_t)sy, (uint16_t)sx);
    unsigned char *d = (unsigned char *)dst + row_at((int16_t)dst_stride, (uint16_t)dst_h, (uint16_t)dy, (uint16_t)dx);
    int32_t s_step = (int32_t)(int16_t)src_stride - (int32_t)(uint16_t)w;
    int32_t d_step = (int32_t)(int16_t)dst_stride - (int32_t)(uint16_t)w;
    uint16_t rows = (uint16_t)h;
    do {
        uint16_t n = (uint16_t)w;
        do
            *d++ = *s++;
        while (--n);
        s += s_step;
        d += d_step;
    } while (--rows);
}

/* 1090:01b4: fill w x h at (left, top) with colour; the step after a row is the stride less w
   computed in 16 bits over a sign-extended stride (`movsx ebx; sub bx, w`) */
void asm_fill_rect(uintptr_t buf, int color, int left, int top, int w, int h, int pitch, int planes)
{
    unsigned char *p = (unsigned char *)(uintptr_t)buf + row_at((int16_t)pitch, (uint16_t)planes, (uint16_t)top, (uint16_t)left);
    uint32_t ext = (uint32_t)(int32_t)(int16_t)pitch;
    uint32_t step = (ext & 0xffff0000u) | (uint16_t)((uint16_t)ext - (uint16_t)w);
    uint16_t rows = (uint16_t)h;
    do {
        uint16_t n = (uint16_t)w;
        do
            *p++ = (unsigned char)color;
        while (--n);
        p += (int32_t)step;
    } while (--rows);
}

/* 1090:0215 and 1090:0348: a sprite through the scale map. The sprite is rows of {word start,
   word count, count bytes}; the scale map (words) gives, for each output row and column, the
   source row and column. Rows are walked forward until the source row reaches the map's;
   a row's pixels are those from its start, the key colour skipped. The mono version writes
   `ink` where the sprite has a pixel. */
static void draw(intptr_t src_l, void *bits, intptr_t map_l, unsigned char key, int px, int py, int stride, int height,
                 int cols, int rows_i, int dx, int dy, int unused_w, int row0, int sx, int sy, int mono, unsigned char ink)
{
    unsigned char *esi = (unsigned char *)(intptr_t)src_l + 4;
    unsigned char *edi = (unsigned char *)bits + row_at((int16_t)stride, (uint16_t)height, (uint16_t)py, (uint16_t)px);
    const int16_t *map = (const int16_t *)(intptr_t)map_l;
    int16_t row_i = (int16_t)row0;                      /* bp-8 */
    const int16_t *rowtab = map + (uint16_t)(dy + sy);  /* bp-a */
    uint16_t rows = (uint16_t)rows_i;
    (void)unused_w;
    do {
        int16_t cx = row_i;
        uint16_t start, count;
        while (cx < *rowtab) {
            esi += (uint32_t)*(const uint16_t *)(esi - 2) + 4;
            cx++;
        }
        row_i = cx;
        start = *(const uint16_t *)(esi - 4);
        count = *(const uint16_t *)(esi - 2);
        if (count != 0) {
            const int16_t *coltab = map + (uint16_t)(dx + sx);
            unsigned char *d = edi;
            int16_t left = (int16_t)cols;
            while ((int16_t)start > *coltab) {
                coltab++;
                d++;
                left--;
            }
            if (left > 0) {
                uint16_t n = (uint16_t)left;
                do {
                    int16_t at = (int16_t)(*coltab - start);
                    unsigned char pixel;
                    if (at >= (int16_t)count)
                        break;
                    pixel = esi[(uint16_t)at];
                    if (pixel != key)
                        *d = mono ? ink : pixel;
                    coltab++;
                    d++;
                } while (--n);
            }
        }
        edi += (int16_t)stride;
        rowtab++;
    } while ((int16_t)--rows > 0);
}

void asm_draw_sprite(intptr_t src, void *bits, intptr_t a, unsigned char col, int px, int py, int bot, int rt, int l, int t,
                     int dx, int dy, int w, int h, int sx, int sy)
{
    draw(src, bits, a, col, px, py, bot, rt, l, t, dx, dy, w, h, sx, sy, 0, 0);
}

void asm_draw_mono_sprite(intptr_t src, void *bits, intptr_t a, unsigned char col, int px, int py, int bot, int rt, int l,
                          int t, int dx, int dy, int w, int h, int sx, int sy, unsigned char mode)
{
    draw(src, bits, a, col, px, py, bot, rt, l, t, dx, dy, w, h, sx, sy, 1, mode);
}

/* 1090:0000: one pass of the dissolve (wipe_rows): for each of `step` table rows from i, and
   each of w columns, the pixel at row (tbl[k] + col[j]) % h of the source goes to the same row
   of the destination, unless it is the key (key >= 0). The column offsets are read at tbl plus
   h BYTES -- the original never reads its `cols` argument, and h bytes is not where wipe_rows
   put them (tbl + h words); kept. Row tables are 32-bit offsets. */
void asm_wipe_rows(intptr_t src_l, intptr_t dst_l, intptr_t cols_l, intptr_t tbl_l, intptr_t src_rows_l, intptr_t dst_rows_l,
                   int i, int w, int h, int step, int left, int top, int x, int y, int key)
{
    unsigned char *src = (unsigned char *)(intptr_t)src_l, *dst = (unsigned char *)(intptr_t)dst_l;
    const unsigned char *tbl = (const unsigned char *)(intptr_t)tbl_l;
    const int32_t *src_rows = (const int32_t *)(intptr_t)src_rows_l;
    const int32_t *dst_rows = (const int32_t *)(intptr_t)dst_rows_l;
    uint16_t remaining = (uint16_t)step;                 /* bp-10 */
    (void)cols_l;
    do {
        uint16_t cx = (uint16_t)w;
        uint16_t k = (uint16_t)((uint16_t)(step - remaining) + (uint16_t)i);
        uint16_t trow = *(const uint16_t *)(tbl + (uint16_t)(k * 2));
        do {
            uint16_t j = (uint16_t)(w - cx);
            uint16_t col = *(const uint16_t *)(tbl + (uint16_t)h + (uint16_t)(j * 2));
            uint16_t r = (uint16_t)((uint16_t)(trow + col) % (uint16_t)h);
            unsigned char pixel = src[(uint32_t)src_rows[(uint16_t)(r + top)] + (uint16_t)left + j];
            if ((int16_t)key < 0 || pixel != (unsigned char)key)
                dst[(uint32_t)dst_rows[(uint16_t)(r + y)] + (uint16_t)x + j] = pixel;
        } while ((int16_t)--cx > 0);
    } while ((int16_t)--remaining > 0);
}
