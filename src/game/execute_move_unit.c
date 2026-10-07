/* PORT: this file's own declarations of move_is_legal, move_unit removed: the header declares them from the definition (W1 step 3). */
/* execute_move_unit: candidate for 1020:e460 (692 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: execute_move_unit
 * size-source: 692, not the census 663 -- the census stops inside the epilogue, and the
 *   trailing bytes are the dead second epilogue and the switch's nine-entry jump table.
 *
 * The gate in front of a unit's move by (dx, dy). `move_is_legal` decides whether the move
 * needs anything said about it; a non-zero answer means it does not, and the move goes
 * straight through. A zero answer leaves the reason in DGROUP:0x4c02, and the nine cases of
 * the switch are the nine things that can be said:
 *
 *   1  NODOCKS        a dialog, and the move does not happen
 *   2  LANDFALL       ask; answer 2 goes ashore
 *   3  LANDFALL2      the same question in the other wording
 *   4  SAILHOME       ask; answer 1 sails for Europe, anything else stays put
 *   5  SAILHOME       the same, but anything but 1 goes through with the move
 *   6  (no text)      redraw the 5x5 around the target and stop
 *   7  SHIPCOMBAT     a message, and the move does not happen
 *   8  SHIPLAKE       the same
 *   9  LANDFIRST      the same
 *
 * With independence declared (bit 0 of SEG20:0x7784) cases 4 and 5 do not ask at all: they
 * show EUROPENOTLEAVE and take answer 2, which is what makes case 4 stop and case 5 proceed.
 *
 * GOING ASHORE WAKES THE CARGO. Answer 2 walks the moving unit's whole chain and clears the
 * orders of every unit whose type has a +8 value under 0x63 -- the passengers -- so a stack
 * that lands is a stack that asks for orders.
 *
 * THE LANDFALL TUTORIAL IS ONE-SHOT AND SPATIAL. After a successful move, unless bit 7 of the
 * player's `NationText+0` is already set, the nine squares around the target are scanned for
 * one that is not ocean; the first hit sets the bit and fires `land_ho`. Both loops
 * carry `found == 0` in their condition, so the scan stops on the first land square.
 *
 * `!(flags & 0x80)` and not `(flags & 0x80) == 0`: the shipped loads the byte, widens it with
 * `MOV AH,0` and tests AL, where the `== 0` form folds to a single `TEST BYTE` (rule 51). The
 * `MOV AH,0` rather than `CBW` is what makes the field `unsigned char`.
 *
 * The whole switch sits inside `if (move_is_legal(...) == 0)`; written as an early
 * `if (... != 0) goto`, the direct `JNZ NEAR` becomes the two-jump form (rule 12).
 *
 * RENAMED 2026-09-26 from unit_move_confirm: the Mac build's execute_move_unit(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller MoveUnit; callee move_unit; callee next_unit; callee popm; callee top_unit), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int popm(char *msg, int n);
extern void unit_activate(int u);
extern int top_unit(int u);
extern int next_unit(int u);
extern void send_unit_to_europe(void);
extern void tell_unit_to_europe(int u);
extern void DrawMapRegion(int x, int y, int a, int b, int flag);

extern int terrain_is_ocean(int x, int y);
extern void land_ho(void);

/* same object: 4-byte `push cs; call ` (rule 47) */


int execute_move_unit(int dx, int dy)
{
    int r;
    int y;
    int tx;
    int ty;
    int answer;
    int u;
    register int x;
    register int found;

    r = 1;
    tx = g_units[g_game.selected_unit].x + dx;
    ty = g_units[g_game.selected_unit].y + dy;
    if (move_is_legal(g_game.selected_unit, tx, ty) == 0) {
    switch (g_move_reason) {
    case 1:
        show_text_dialog_1020_e73b("NODOCKS");
        goto out;
    case 2:
    case 3:
        if (g_move_reason == 2)
            answer = popm("LANDFALL", 3);
        else
            answer = popm("LANDFALL2", 3);
        if (answer != 2)
            goto out;
        unit_activate(g_move_reason_unit);
        u = top_unit(g_game.selected_unit);
        while (u >= 0) {
            if (g_unit_types[g_units[u].type].size < 0x63)
                g_units[u].orders = ORDERS_NONE;
            u = next_unit(u);
        }
        break;
    case 4:
    case 5:
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            show_text_dialog_1020_e73b("EUROPENOTLEAVE");
            answer = 2;
        } else {
            answer = popm("SAILHOME", 0);
        }
        if (answer == 1) {
            send_unit_to_europe();
            tell_unit_to_europe(g_game.selected_unit);
            goto out;
        } else if (g_move_reason == 4) {
            goto out;
        }
        break;
    case 6:
        DrawMapRegion(tx - 2, ty - 2, 5, 5, 1);
        goto out;
    case 7:
        popm("SHIPCOMBAT", 0);
        goto out;
    case 8:
        popm("SHIPLAKE", 0);
        goto out;
    case 9:
        popm("LANDFIRST", 0);
        goto out;
    default:
        goto out;
    }
    }
    move_unit(g_game.selected_unit, tx, ty);
    r = 0;
    set_cursor_pos(tx, ty);
    if (!(g_nation_text[g_game.turn_nation].once_flags & 0x80)) {
        found = 0;
        for (y = ty - 1; found == 0 && ty + 1 >= y; y++)
            for (x = tx - 1; found == 0 && tx + 1 >= x; x++)
                if (terrain_is_ocean(x, y) == 0) {
                    g_nation_text[g_game.turn_nation].once_flags |= 0x80;
                    found = 1;
                    land_ho();
                }
    }
out:
    return r;
}
