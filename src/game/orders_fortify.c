/* orders_fortify: candidate for 1008:0278 (319 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Fortifies the arrived unit -- orders 5, the counter at +0x16 cleared, a sound and a redraw --
 * but ASKS FIRST if it would be fortifying next to someone the nation has a treaty with. The
 * eight neighbouring squares are scanned for a European nation that is not this one and whose
 * pair flags carry bit 6; the first such nation stops the scan, and the message key at
 * DGROUP:0x15e is "HAVETREATY". Anything but answer 2 abandons the whole thing.
 *
 * `found` at BP-0x12 is set and never read -- the scan's real output is `other`, and the
 * variable beside it is dead. It is in the shipped code.
 *
 * `n` OUTLIVES THE LOOP. The test after it compares the nation against SI, which holds
 * whatever the last `is_colony` returned -- not necessarily the nation `other`
 * records. When the loop breaks they agree; when it runs out they need not, and the second half
 * of the condition is then reading the eighth neighbour.
 *
 * Every branch in the tail is the two-jump form (rule 12): the guard before the message, the
 * answer test, and the `return` -- which is a jump to the function's end rather than an inline
 * epilogue, so it is a `goto` and the function has ONE exit.
 *
 * RENAMED 2026-09-26 from fortify_unit: the Mac build's orders_fortify(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee end_unit_turn; callee get_treaty; callee is_colony; callee popm), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "units.h"
#include "sounds.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameState g_game;   /* SEG20:0x7782 */

extern int is_colony(int x, int y);
extern int get_treaty(int nation_a, int nation_b);
extern int get_name(int nation);
extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern void sound_play(int n);
extern void end_unit_turn(int u);

void orders_fortify(void)
{
    int u;
    int nation;
    int other;
    int x;
    int y;
    int nx;
    int ny;
    int found;
    register int n;
    register int i;

    u = g_game.selected_unit;
    nation = NATION(g_units[u]);
    x = g_units[u].x;
    y = g_units[u].y;
    found = 0;
    other = -1;
    for (i = 0; i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        n = is_colony(nx, ny);
        if (n < 0 || n == nation)
            continue;
        if ((get_treaty(nation, n) & 0x40) == 0)
            continue;
        found = 1;
        other = n;
        break;
    }
    if (other < 0 || nation == n)
        goto fortify;
    popup_say(0, get_name(n));
    if (popm("HAVETREATY", 1) != 2)
        goto out;
fortify:
    sound_play(SOUND_COCKGUN);
    g_units[u].orders = ORDERS_FORTIFY;
    g_units[u].counter = 0;
    end_unit_turn(u);
out:
    ;
}
