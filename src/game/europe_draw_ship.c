/* europe_draw_ship -- shipped at 1048:993a.
 *
 * Ported by tools/port.py from matched/game/europe_draw_ship.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern char far g_icon_sprites[][0x20];              SEG20:0x6246
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *
 * The file's own Unit, UnitType, Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_at_8_1048_e0ee(void *o);
void classify_index_into_tier(col_i16 idx, col_i16 base_x, col_i16 tier3_count, col_i16 extra,
                                  col_i16 *tier, col_i16 *x, col_i16 *y,
                                  col_i16 *w, col_i16 *h);
void set_current_nation(col_i16 n);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "units.h"
extern col_i16 unit_portrait(col_i16 u);
extern void draw_unit_icon(Win *w, col_i16 u, col_i16 a, col_i16 x, col_i16 y, col_i16 b, col_i16 c);
extern void scale_map_select(col_i16 a, col_i16 b);
extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern void sprite_draw(RECT *out, void *rec, Win *dst, col_i16 x, col_i16 y);


extern void TextColor(col_i16 c);
extern void GRPort_DrawRect(Win *w, RECT *r);
/* same object: 4-byte `push cs; call ` (rule 47) */




void europe_draw_ship(Win *w, col_i16 u, col_i16 base_x, col_i16 count3, col_i16 extra,
                                  col_i16 *n, col_i16 h)
{
    col_i16 tier;
    col_i16 cx;
    col_i16 cy;
    col_i16 cw;
    col_i16 ch;
    RECT r;
    RECT s1;
    RECT s2;
    register col_i16 unit = u;
    register col_i16 half;

    set_current_nation(w->nation);
    classify_index_into_tier(*n, base_x, count3, extra, &tier, &cx, &cy, &cw, &ch);
    half = rect_width_at_8_1048_e0ee((&g_icon_sprites[unit_portrait(unit)])) / 2 + 5;
    if (tier < 2) {
        if (tier == 0) {
            draw_unit_icon(w, unit, 0, w->ox + half + cx - (tier == 1 ? 4 : 0), w->oy + cy, 0x20, 0x64);
        } else {
            scale_map_select(1, 2);
            draw_unit_icon(w, unit, 0, w->ox + half + cx - (tier == 1 ? 4 : 0), w->oy + cy, 0x20, 0x64);
            scale_map_select(1, 1);
        }
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST
            && tier == 0 && extra < 2 && g_units[unit].cargo_count != 0) {
            scale_map_select(3, 4);
            sprite_draw(&s1, (&g_goods_sprites[unit_cargo_type(unit, 0)]), w,
                        w->ox + half + cx - 10, w->oy + cy - 5);
            scale_map_select(1, 1);
        }
    } else if (tier < 3) {
        scale_map_select(100 / (tier * 2), 100);
        sprite_draw(&s2, (&g_icon_sprites[g_unit_types[g_units[unit].type].icon]), w,
                    w->ox + half + cx + (cw >> 1) - 1, w->oy + cy + ch - 1);
        scale_map_select(1, 1);
    }
    if (h >= 0 && tier < 3) {
        SetRect(&r, cx, cy - 1, cx + cw, cy + ch);
        OffsetRect(&r, w->ox, w->oy);
        TextColor(0);
        GRPort_DrawRect(w, &r);
    }
    (*n)++;
}
