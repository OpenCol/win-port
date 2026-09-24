/* draw_window_frame -- shipped at 1048:077f.
 *
 * Ported by tools/port.py from matched/game/draw_window_frame.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_art_base[][0x20];                  SEG20:0x41a6
 *   extern char far g_title_font;                        DGROUP:0x4b06
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_height_1048_d7c5(Win *w);
col_i16 port_width_1048_d4f6(Win *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal InflateRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void get_rect_at_8(Win *o, RECT *r);
extern void port_apply_rect(Win *w);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(RECT *out, void *rec, void *dst, col_i16 x, col_i16 y);


extern void draw_bevel(Win *w, RECT *r, col_i16 depth, col_i16 c1, col_i16 c2);
extern void TextColor(col_i16 c);
extern void GRPort_DrawRect(Win *w, RECT *r);
extern void draw_string_shadowed(Win *w, void *font, char *s,
                                     col_i16 x, col_i16 y, col_i16 a);
extern void port_set_clip(Win *w, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */



void draw_window_frame(Win *w)
{
    col_i16 cw;
    col_i16 ch;
    col_i16 gx;
    col_i16 gy;
    RECT r;
    RECT clip;
    RECT t1;
    RECT t2;
    RECT t3;
    RECT t4;
    RECT t5;
    RECT t6;
    register col_i16 col;
    register col_i16 row;

    get_rect_at_8(w, &clip);
    port_apply_rect(w);
    cw = port_width_1048_d4f6(w);
    ch = port_height_1048_d7c5(w);
    gx = cw / w->tile_w * w->tile_w;
    gy = ch / w->tile_h * w->tile_h;
    scale_map_select(1, 1);
    for (row = 0; ch / w->tile_h + 1 > row; row++) {
        sprite_draw(&t1, (&g_art_base[411]), w, 0, w->tile_h * row);
        sprite_draw(&t2, (&g_art_base[411]), w, gx, w->tile_h * row);
        sprite_draw(&t3, (&g_art_base[411]), w, gx - w->tile_w, w->tile_h * row);
    }
    for (col = 0; cw / w->tile_w + 1 > col; col++) {
        sprite_draw(&t4, (&g_art_base[411]), w, w->tile_w * col, 0);
        sprite_draw(&t5, (&g_art_base[411]), w, w->tile_w * col, gy);
        sprite_draw(&t6, (&g_art_base[411]), w, w->tile_w * col, gy - w->tile_h);
    }
    SetRect(&r, 0, 0, cw, ch);
    draw_bevel(w, &r, 2, 0x16, 0x12);
    SetRect(&r, w->ox - 2, w->oy - 2, w->wr + 2, w->wb + 2);
    draw_bevel(w, &r, -2, 0xff, 0xf8);
    InflateRect(&r, 1, 1);
    TextColor(0);
    GRPort_DrawRect(w, &r);
    TextColor(0xc);
    draw_string_shadowed(w, &g_title_font, ((char *)((unsigned char *)(w) + 0xc4)), cw / 2, 0x12, 0x10);
    TextColor(0);
    port_set_clip(w, &clip);
}
