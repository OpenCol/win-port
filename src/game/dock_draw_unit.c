/* dock_draw_unit -- shipped at 1048:9bfc.
 *
 * Ported by tools/port.py from matched/game/dock_draw_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Sprite far g_goods_sprites[];                 SEG20:0x6526
 *
 * The file's own Sprite, Unit, Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void grid_cell_layout(col_i16 a, col_i16 b, col_i16 *band, col_i16 *pos,
                                 col_i16 *width, col_i16 *p6, col_i16 *p7);
void set_current_nation(col_i16 n);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "units.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern void sprite_draw(RECT *out, Sprite *s, Win *o, col_i16 x, col_i16 y);
extern void GRPort_DrawRect(Win *w, RECT *r);
extern void TextColor(col_i16 c);

extern void draw_unit_icon(Win *w, col_i16 u, col_i16 a, col_i16 x, col_i16 y, col_i16 b, col_i16 c);

/* same object: 4-byte `push cs; call ` */


void dock_draw_unit(Win *w, register col_i16 u, col_i16 x, col_i16 *n, col_i16 h)
{
    col_i16 band;
    col_i16 px;
    col_i16 py;
    col_i16 d;
    col_i16 e;
    RECT r;
    RECT s;

    set_current_nation(w->nation);
    grid_cell_layout(*n, x, &band, &px, &py, &d, &e);
    if (band < 2) {
        draw_unit_icon(w, u, 0, w->ox + px, w->oy + py, 0x20, 0x64);
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && band == 0
            && g_units[u].cargo_count != 0)
            sprite_draw(&s, &g_goods_sprites[unit_cargo_type(u, 0)], w,
                        w->ox + px, w->ox + py);
    }
    if (h >= 0 && band < 2) {
        SetRect(&r, w->ox + px - 0x10, w->oy + py, w->ox + px + 0x10, w->oy + py + 0x20);
        TextColor(0);
        GRPort_DrawRect(w, &r);
    }
    (*n)++;
}
