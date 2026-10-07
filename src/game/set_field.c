/* PORT: this file's own declarations of village_bribe removed: the header declares it from the definition (W1 step 3). */
/* set_field: candidate for 1018:4280 (613 bytes).  Verdict: RELOCATABLE.
 * symbol: set_field
 * name-source: sibling-build
 *
 * Puts colonist `c` on square (col, row) of the current colony's 5x5 work area -- the byte at
 * Colony+0x70 indexed through `field_index` -- and, when a colonist really goes to
 * work there, TAKES THE LAND. `get_field` is the matching reader, and
 * `pick_up_colonist` calls this with -1 to empty a square.
 *
 * Nothing but the store happens for -1, or while DGROUP:0xc23 is set (1018:7327 sets it around
 * its own rearranging). Otherwise an unowned square gets the colony's nation in map plane 2's
 * high nibble, and a square a tribe still claims (the SEG21:0x213 table colony_indian_table
 * fills) is settled one of two ways:
 *
 *   - BOUGHT, for a native nation or a European one whose SEG20:0x7841 control byte is non-zero:
 *     `village_bribe` names a price, and a treasury left with at least half the
 *     price again pays it and bumps the tribe record's count.
 *   - TAKEN otherwise, or when the treasury is short: `piss_off` raises the tribe's
 *     alarm by 5 plus the difficulty (for a human European), doubled when the nearest village is
 *     within 2 squares, tripled within 1, and doubled again when `resource_at` finds
 *     something on the square.
 *
 * Either way the claim is cleared to NATION_NONE, and `set_indian` marks the
 * square.
 *
 * NAMING THE NUMBERS SETTLED AN ARGUMENT ORDER ELSEWHERE. The tribe is passed to
 * `piss_off` as `claim - NATION_EUROPEAN_COUNT` -- a nation index turned into a
 * tribe index -- as its FIRST argument. That file had the first parameter named `nation`; its body
 * uses it as `tribe + NATION_EUROPEAN_COUNT` and halves the rise for `nation == NATION_FRANCE`, so it
 * is corrected there (RE_STATUS 343).
 *
 * `(int)c >= 0` is rule 51: the shipped widens the char parameter before the test. The price is
 * compared with `price >> 1`, a shift, not `price / 2`. The alarm is doubled in the accumulator,
 * `total * 2` (rule 63). The census size is exact: 1018:44e5 follows immediately.
 *
 * RENAMED 2026-09-26 from colony_set_cell: the Mac build's set_field(short, short, char), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller colony_select_profession; caller pick_up_colonist; callee find_close_village; callee is_city), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int land_region_at(int x, int y);
extern int is_unit(int x, int y);
extern int is_city(int x, int y);
extern void owner_set(int x, int y, int n);
extern int find_close_village(int x, int y, int nation, int region);
extern int resource_at(int x, int y);
extern void piss_off(int tribe, int nation, int delta, int unused);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int field_index(int x, int y);
extern long check_gold(int nation);
extern void subtract_gold(int nation, long amount);
extern void set_indian(int x, int y, int bits);

void set_field(int col, int row, char colonist)
{
    int idx;
    int claim;
    int base;
    int alarm;
    int total;
    int price;
    int region;
    register int x;
    register int y;

    idx = field_index(col, row);
    region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (idx >= 0) {
        g_cur_colony_rec->slots[idx] = colonist;
        if ((int)colonist >= 0 && g_trial_placement == 0) {
            x = g_cur_colony_rec->x + col - 2;
            y = g_cur_colony_rec->y + row - 2;
            if (is_unit(x, y) < 0 && is_city(x, y) < 0)
                owner_set(x, y, g_cur_colony_rec->nation);
            claim = g_tile_tribe[col][row];
            if (claim >= 0) {
                find_close_village(x, y, -1, region);
                price = 0;
                if (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT
                 || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN) {
                    price = village_bribe(g_cur_settlement, g_cur_colony_rec->nation, x, y);
                    if (check_gold(g_cur_colony_rec->nation) - price >= price >> 1) {
                        g_cur_tribe_rec->count++;
                        subtract_gold(g_cur_colony_rec->nation, (long)price);
                    } else
                        price = 0;
                }
                if (price == 0) {
                    base = (g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN
                            ? g_game.difficulty : 0) + 5;
                    alarm = base;
                    if (g_nearest_dist <= 2)
                        alarm += base;
                    if (g_nearest_dist <= 1)
                        alarm += base;
                    total = alarm;
                    if (resource_at(x, y) != -1)
                        total = total * 2;
                    piss_off(claim - NATION_EUROPEAN_COUNT, g_cur_colony_rec->nation, total, 0);
                }
                g_tile_tribe[col][row] = NATION_NONE;
            }
            set_indian(col, row, 1);
        }
    }
}
