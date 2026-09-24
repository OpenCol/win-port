/* draw_unit_marks -- shipped at 1038:a99f.
 *
 * Ported by tools/port.py from matched/game/draw_unit_marks.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void scale_map_select(col_i16 a, col_i16 b);
extern void port_set_clip(void *w, void *r);
extern void sprite_draw9(RECT *out, void *sprite, void *w, col_i16 x, col_i16 y, unsigned char c);
extern void sprite_draw(RECT *out, void *sprite, void *w, col_i16 x, col_i16 y);
extern void port_apply_rect(void *w);

void draw_unit_marks(MapWin *w, void *sprite, col_i16 x, col_i16 y, col_i16 flags)
{
    col_i16 z;
    col_i16 eight;
    RECT r1;
    RECT r2;
    RECT r3;

    z = w->zoom + 8;
    eight = 8;
    scale_map_select(z, eight);
    port_set_clip(w, &w->ox);
    if (flags & 8)
        sprite_draw9(&r1, sprite, w, w->ox + x + 2, w->oy + y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 1)
        sprite_draw9(&r2, sprite, w, w->ox + x - 2, w->oy + y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        sprite_draw(&r3, sprite, w, w->ox + x, w->oy + y);
    port_apply_rect(w);
    scale_map_select(1, 1);
}
