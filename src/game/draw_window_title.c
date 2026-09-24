/* draw_window_title -- shipped at 1048:0ecf.
 *
 * Ported by tools/port.py from matched/game/draw_window_title.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Sprite far g_background_tile;                 SEG20:0x7506
 *   extern char far g_title_font;                        DGROUP:0x4b06
 *
 * The file's own Sprite, Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1048_d4f6(Obj *o);
col_i16 get_w_4_1048_d7c5(Obj *o);
void copy_rect_to_screen(Obj *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void get_rect_at_8(Obj *o, RECT *r);
extern void port_set_clip(Obj *p, RECT *r);
extern void get_scale_pair(col_i16 *a, col_i16 *b);
extern void scale_map_select(col_i16 w, col_i16 h);
extern void sprite_draw(RECT *out, Sprite *s, Obj *port, col_i16 x, col_i16 y);
extern void TextColor(col_i16 v);
extern void draw_string_shadowed(Obj *p, void *font, char *s,
                                     col_i16 x, col_i16 y, col_i16 align);
extern void port_apply_rect(Obj *p);


/* same object: 4-byte `push cs; call ` (rule 47) */




void draw_window_title(Obj *w)
{
    RECT r;
    RECT clip;
    col_i16 sx;
    col_i16 sy;
    col_i16 n;
    RECT out;
    col_i16 j;
    col_i16 i;

    if (get_w_4_1048_d7c5(w) != 0 && get_w_2_1048_d4f6(w) != 0) {
        get_rect_at_8(w, &clip);
        SetRect(&r, 2, 2, get_w_2_1048_d4f6(w) - 2, 0x1e);
        port_set_clip(w, &r);
        get_scale_pair(&sx, &sy);
        scale_map_select(1, 1);
        n = get_w_2_1048_d4f6(w) / w->tile_w + 1;
        for (j = 0; j < 2; j++)
            for (i = 0; i < n; i++)
                sprite_draw(&out, &g_background_tile, w, (col_i32)i * w->tile_w, (col_i32)j * w->tile_h);
        TextColor(0xc);
        draw_string_shadowed(w, &g_title_font, ((char *)((unsigned char *)(w) + 0xc4)), get_w_2_1048_d4f6(w) / 2, 0x12, 0x10);
        TextColor(0);
        port_apply_rect(w);
        port_set_clip(w, &clip);
        copy_rect_to_screen(w, &r);
        scale_map_select(sx, sy);
    }
}
