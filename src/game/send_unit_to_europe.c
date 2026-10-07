/* send_unit_to_europe: candidate for 1030:2a9e (470 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: send_unit_to_europe
 *
 * The selected unit and everything stacked with it become present at a square. Takes no
 * arguments -- everything comes from `g_game.selected_unit` (SEG20:0x7794).
 *
 * The square is read from the unit FIRST and kept in locals, because
 * `stack_units` runs next and may move it; every later use is of the
 * square as it was on entry. That ordering is the function's one subtlety and the reason
 * the two bytes are copied into locals at all rather than re-read.
 *
 * It publishes that square three ways:
 *   - into the owning nation's 316-byte record, at +0x2b and +0x2c of the array
 *     `has_father` indexes;
 *   - into +9 and +0xa of EVERY unit in the stack, walking from `top_unit` through
 *     `next_unit`, together with `get_voyage_delay`'s result in +0x16;
 *   - and to the display, through `DrawMapRegion` on that one square.
 *
 * Orders are set as the stack walks: the selected unit is woken (0) unless it is already
 * in state 2, and everything else in the stack is put in state 1. Then
 * `unspot_stack`, `relocate_stacked_units` against the square
 * `(g_game.turn_nation - 12, g_game.turn_nation - 12)`, and `possibly_spot_stack`.
 *
 * That relink square is off the map by construction and is NOT the Europe square: this
 * build puts Europe at `(nation - 20, nation - 20)` -- `num_cargo_vessels` walks the
 * chain there and `europe_new_wars` musters the King's Regulars there -- so
 * `(nation - 12)` is a second off-board region, eight squares away in both coordinates.
 * The three callers are `orders_europe`, the `LANDFALL` message and `perform_orders_move_to`,
 * which are the sailing transitions, so a high-seas holding area fits; that is an
 * inference from the callers and not something these bytes prove.
 *
 * The nation record is indexed through `g_nation_bits[][0x13c]`, the same declaration
 * `matched/has_father.c` uses, and that is deliberate. Two matched files state base
 * addresses for this one array that are 0x30 apart -- `europe_new_wars.c` says
 * `SEG20:0xc2c4` with its flag byte at +0x30, `check_gold.c` says `0xc2f4` with
 * gold at +0x2a -- and BOTH are self-consistent, because each one's stated offset lands on
 * the absolute address its own shipped instruction reads (`0xc2f4` and `0xc31e`). So the
 * disagreement is about where the record STARTS, which neither file's bytes can settle.
 * The lowest 0x13c-strided access anywhere in the binary is `0xc2f4`, which favours the
 * higher base without proving it. Indexing from the base `has_father` already
 * establishes (absolute `0xc2fb`) adds no third claim: the fields here are at `+0x2b` and
 * `+0x2c` from it, which is `+0x32`/`+0x33` of the record if the base is `0xc2f4`.
 *
 * The nation nibble is masked in 8 bits before the index promotes it (rule 27).
 *
 * RENAMED 2026-09-26 from unit_stack_arrives: the Mac build's send_unit_to_europe(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between get_voyage_delay and set_course_for_europe, which are already bound; the arity agrees and the Mac body is 0.86x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern Nation g_nations[];   /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */
extern void stack_units(int u);
extern int top_unit(int u);
extern int next_unit(int u);
extern void unspot_stack(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern void possibly_spot_stack(int u);
extern void DrawMapRegion(int x, int y, int a, int b, int flag);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */
extern int get_voyage_delay(int u, int x, int y);

void send_unit_to_europe(void)
{
    int x;
    int y;
    int r;
    register int u;

    x = g_units[g_game.selected_unit].x;
    y = g_units[g_game.selected_unit].y;
    g_nations[(unsigned char)(g_units[g_game.selected_unit].flags & UNIT_NATION_MASK)].start_x = (unsigned char)x;
    g_nations[(unsigned char)(g_units[g_game.selected_unit].flags & UNIT_NATION_MASK)].start_y = (unsigned char)y;
    stack_units(g_game.selected_unit);
    r = get_voyage_delay(g_game.selected_unit, x, y);
    u = top_unit(g_game.selected_unit);
    while (u >= 0) {
        if (g_game.selected_unit == u) {
            if (g_units[u].orders != ORDERS_TRADE_ROUTE)
                g_units[u].orders = ORDERS_NONE;
        } else {
            g_units[u].orders = ORDERS_SENTRY;
        }
        g_units[u].dest_x = (unsigned char)x;
        g_units[u].dest_y = (unsigned char)y;
        g_units[u].counter = (char)r;
        u = next_unit(u);
    }
    unspot_stack(g_game.selected_unit);
    relocate_stacked_units(g_game.selected_unit, g_game.turn_nation - 12, g_game.turn_nation - 12);
    possibly_spot_stack(g_game.selected_unit);
    DrawMapRegion(x, y, 1, 1, 1);
}
