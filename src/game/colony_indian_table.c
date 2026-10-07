/* colony_indian_table: candidate for 1018:7119 (459 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * WHICH OF THE COLONY'S 25 SQUARES A TRIBE STILL CLAIMS. For each square of the 5x5 around the
 * current colony, the owner starts as none (-1) and becomes the tribe of the nearest village
 * in the colony's own land region -- but only while that village is close enough, its distance
 * no more than the tribe's level. Then five things take the claim away again: a colonist
 * already working the square, water, plane-1 bit 4 on the square, a tribe this nation has no
 * 0x20 pair-flag with, and nation flag 2 on the colony's owner, which clears every claim.
 *
 * The result goes into BOTH 5x5 tables at SEG21:0x213 and SEG21:0x1fa in one chained store --
 * the shipped keeps the byte in DL, writes 0x213 and then copies DL to AL for 0x1fa -- and a
 * surviving claim on a square nobody has marked or owns is written into map plane 2's high
 * nibble, so the map remembers it.
 *
 * WHICH EXTERN RECEIVES WHICH STORE IS NOT SOMETHING THE COMPARATOR CHECKS: both displacements
 * are fixups. The order here comes from the disassembly, 0x213 first (§119, RE_STATUS 250).
 *
 * `(int)get_field(i, j) >= 0`: the helper returns a `char` and the shipped
 * `CBW`s it before `OR AX,AX`; without the cast the test folds to `OR AL,AL` (rule 51). Three
 * helpers come through the 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from colony_native_claims: the Mac build's colony_indian_table(void), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller Show; caller create_colony; caller new_turn_colony; callee has_father; callee land_region_at), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "fathers.h"
#include "records.h"

extern int land_region_at(int x, int y);

extern int find_close_village(int x, int y, int nation, int region);
extern int tribe_radius(int tribe);
extern int terrain_is_ocean(int x, int y);
extern int get_treaty(int nation_a, int nation_b);
extern int has_father(int nation, int bit);
extern int is_unit(int x, int y);
extern int is_city(int x, int y);
extern void owner_set(int x, int y, int v);

/* same object: 4-byte `push cs; call ` */
extern void colony_field_table(void);
extern char get_field(int x, int y);
extern int get_indian(int x, int y);

void colony_indian_table(void)
{
    int x;
    int y;
    int owner;
    int village;
    int lvl;
    int region;
    register int i;
    register int j;

    region = land_region_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    colony_field_table();
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++) {
            owner = -1;
            x = g_cur_colony_rec->x + i - 2;
            y = g_cur_colony_rec->y + j - 2;
            if (on_map(x, y) != 0) {
                village = find_close_village(x, y, -1, region);
                if (village >= 0) {
                    lvl = tribe_radius(g_cur_tribe);
                    if (g_nearest_dist <= lvl)
                        owner = g_cur_settlement_rec->owner;
                }
            }
            if ((int)get_field(i, j) >= 0)
                owner = -1;
            if (terrain_is_ocean(x, y) != 0)
                owner = -1;
            if (get_indian(i, j) != 0)
                owner = -1;
            if (owner >= 0 && (get_treaty(g_cur_colony_rec->nation, owner) & 0x20) == 0)
                owner = -1;
            if (has_father(g_cur_colony_rec->nation, FATHER_PETER_MINUIT) != 0)
                owner = -1;
            g_cell_claim[i][j] = g_tile_tribe[i][j] = owner;
            if (owner >= 0 && g_grid_marks[i][j] == 0 && is_unit(x, y) < 0
                && is_city(x, y) < 0)
                owner_set(x, y, owner);
        }
    }
}
