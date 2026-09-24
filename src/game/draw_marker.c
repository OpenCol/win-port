/* draw_marker -- shipped at 1038:a849.
 *
 * Ported by tools/port.py from matched/game/draw_marker.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_icon_sprites[][0x20];              SEG20:0x6246
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: zoom -> right
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void scale_map_select(col_i16 a, col_i16 b);
extern void port_set_clip(Win *w, RECT *r);
extern void port_apply_rect(Win *w);
extern void sprite_draw(RECT *out, void *sprite, Win *o, col_i16 x, col_i16 y);
extern void sprite_draw9(RECT *out, void *sprite, Win *o, col_i16 x, col_i16 y, unsigned char c);

void draw_marker(Win *w, register col_i16 spr, col_i16 x, col_i16 y, register col_i16 flags)
{
    col_i16 scale;
    col_i16 base;
    RECT r1;
    RECT r2;
    RECT r3;

    scale = w->right + 8;
    base = 8;
    scale_map_select(scale, base);
    port_set_clip(w, &(*((RECT *)((unsigned char *)(w) + 0x76a))));
    if (flags & 8)
        sprite_draw9(&r1, (&g_icon_sprites[spr]), w, (*((RECT *)((unsigned char *)(w) + 0x76a))).left + x + 2, (*((RECT *)((unsigned char *)(w) + 0x76a))).top + y,
                     (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 1)
        sprite_draw9(&r2, (&g_icon_sprites[spr]), w, (*((RECT *)((unsigned char *)(w) + 0x76a))).left + x - 2, (*((RECT *)((unsigned char *)(w) + 0x76a))).top + y,
                     (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        sprite_draw(&r3, (&g_icon_sprites[spr]), w, (*((RECT *)((unsigned char *)(w) + 0x76a))).left + x, (*((RECT *)((unsigned char *)(w) + 0x76a))).top + y);
    port_apply_rect(w);
    scale_map_select(1, 1);
}
