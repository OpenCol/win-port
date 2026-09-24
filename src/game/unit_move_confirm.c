/* unit_move_confirm -- shipped at 1020:e460.
 *
 * Ported by tools/port.py from matched/game/unit_move_confirm.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int g_move_reason;                            DGROUP:0x4c02
 *   extern int g_move_reason_unit;                       DGROUP:0x4c04
 *
 * The file's own Unit, UnitType, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 move_reason(col_i16 u, col_i16 x, col_i16 y);
col_i16 show_text_dialog_1020_e73b(char *s);
void unit_move_to_square(col_i16 u, col_i16 x, col_i16 y);

#include "units.h"
#include "gameflag.h"
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void unit_arrives(col_i16 u);
extern col_i16 unit_chain_tail(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);
extern void unit_stack_arrives(void);
extern void viceroy_unit_msg5(col_i16 u);
extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 a, col_i16 b, col_i16 c);
extern void set_cursor_pos(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern void tutorial_landfall(void);

/* same object: 4-byte `push cs; call ` (rule 47) */




col_i16 unit_move_confirm(col_i16 dx, col_i16 dy)
{
    col_i16 r;
    col_i16 y;
    col_i16 tx;
    col_i16 ty;
    col_i16 answer;
    col_i16 u;
    register col_i16 x;
    register col_i16 found;

    r = 1;
    tx = g_units[g_selected_unit].x + dx;
    ty = g_units[g_selected_unit].y + dy;
    if (move_reason(g_selected_unit, tx, ty) == 0) {
    switch (g_move_reason) {
    case 1:
        show_text_dialog_1020_e73b("NODOCKS");
        goto out;
    case 2:
    case 3:
        if (g_move_reason == 2)
            answer = text_message_show("LANDFALL", 3);
        else
            answer = text_message_show("LANDFALL2", 3);
        if (answer != 2)
            goto out;
        unit_arrives(g_move_reason_unit);
        u = unit_chain_tail(g_selected_unit);
        while (u >= 0) {
            if (g_unit_types[g_units[u].type].size < 0x63)
                g_units[u].orders = ORDERS_NONE;
            u = unit_chain_next(u);
        }
        break;
    case 4:
    case 5:
        if (g_game_flags & GAMEFLAG_INDEPENDENCE) {
            show_text_dialog_1020_e73b("EUROPENOTLEAVE");
            answer = 2;
        } else {
            answer = text_message_show("SAILHOME", 0);
        }
        if (answer == 1) {
            unit_stack_arrives();
            viceroy_unit_msg5(g_selected_unit);
            goto out;
        } else if (g_move_reason == 4) {
            goto out;
        }
        break;
    case 6:
        redraw_map_area(tx - 2, ty - 2, 5, 5, 1);
        goto out;
    case 7:
        text_message_show("SHIPCOMBAT", 0);
        goto out;
    case 8:
        text_message_show("SHIPLAKE", 0);
        goto out;
    case 9:
        text_message_show("LANDFIRST", 0);
        goto out;
    default:
        goto out;
    }
    }
    unit_move_to_square(g_selected_unit, tx, ty);
    r = 0;
    set_cursor_pos(tx, ty);
    if (!(g_nation52[g_turn_nation].f0 & 0x80)) {
        found = 0;
        for (y = ty - 1; found == 0 && ty + 1 >= y; y++)
            for (x = tx - 1; found == 0 && tx + 1 >= x; x++)
                if (ocean(x, y) == 0) {
                    g_nation52[g_turn_nation].f0 |= 0x80;
                    found = 1;
                    tutorial_landfall();
                }
    }
out:
    return r;
}
