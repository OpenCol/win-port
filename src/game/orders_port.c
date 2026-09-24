/* orders_port -- shipped at 1008:14bf.
 *
 * Ported by tools/port.py from matched/game/orders_port.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int far g_game_flags;                         SEG20:0x7784
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 show_text_dialog_1008_cdf3(char *msg);

#include "gameflag.h"
/* 0x1c bytes at SEG20:0xa208 */
/* 0xca at SEG20:0x7c28 */




extern col_i16 pick_trade_route_stop(col_i16 unit, col_i16 cur, col_i16 mark_here, col_i16 offer_extra);
extern void unit_head_for_sea_lane(col_i16 unit);
extern void set_current_colony(col_i16 c);

/* same object: 4-byte `push cs; call ` */


void orders_port(void)
{
    register col_i16 c;
    register col_i16 unit;

    unit = g_selected_unit;
    c = pick_trade_route_stop(unit, -1, 1, 0);
    if (c == 0x3e7) {
        if (g_game_flags & GAMEFLAG_INDEPENDENCE)
            show_text_dialog_1008_cdf3("EUROPENOTLEAVE");
        else
            unit_head_for_sea_lane(unit);
    } else if (c >= 0) {
        set_current_colony(c);
        g_units[unit].orders = 3;
        g_units[unit].dest_x = g_colonies[c].x;
        g_units[unit].dest_y = g_colonies[c].y;
    }
}
