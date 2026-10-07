/* colony_cargo_click -- shipped at 1010:52ff.
 *
 * Ported by tools/port.py from matched/game/colony_cargo_click.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_selected_carrier;                   DGROUP:0x0c16
 *   extern unsigned char far g_drag_goods;               DGROUP:0x4baf
 *   extern unsigned char far g_drag_amount;              DGROUP:0x4bb0
 *   extern unsigned char far g_drag_slot;                DGROUP:0x4bb1
 *   extern char far g_goods_sprites[][32];               SEG20:0x6526
 *   extern int g_stub_a;                                 
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 hittest_panel(col_i16 x, col_i16 y);

extern void set_current_colony(col_i16 c);
extern col_i16 colony_nth_cargo_unit(col_i16 n);
extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
extern void goods_toggle_msg(void *w, col_i16 goods, col_i16 unused, col_i16 flag);
extern void ColChild_InitCWin(void *w, void *sprite, col_i16 x, col_i16 y, col_i16 *rx, col_i16 *ry);
extern col_i16 should_ask_quantity(void);
extern col_i16 warehouse_capacity_check(void *w, col_i16 unit, col_i16 goods, col_i16 ask);
extern void colony_dock_click(void *w, col_i16 x, col_i16 y);



void colony_cargo_click(ColChild *w, col_i16 px, col_i16 py)
{
    register col_i16 u;
    col_i16 rx;
    col_i16 ry;
    col_i16 rel;
    col_i16 slot;
    col_i16 goods;

    set_current_colony(w->colony);
    rel = px - 0xfc;
    slot = rel / 24;
    u = colony_nth_cargo_unit(g_selected_carrier);
    goods = unit_cargo_type(u, slot);
    if (goods >= 0) {
        g_drag_goods = goods;
        g_drag_slot = slot;
        g_drag_amount = unit_cargo_amount(u, slot);
        goods_toggle_msg(w, goods, 0, 0);
        ColChild_InitCWin(w, (&g_goods_sprites[g_drag_goods]), px, py, &rx, &ry);
        if (hittest_panel(rx, ry) == 5) {
            warehouse_capacity_check(w, u, g_drag_goods, should_ask_quantity());
            goto done;
        } else if (hittest_panel(rx, ry) == 8) {
            w->panel = 7;
            colony_dock_click(w, rx, ry);
            w->panel = 0;
        }
    }
done:
    ;
}
