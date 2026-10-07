/* indian_turns: candidate for 1030:a541 (379 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: indian_turns
 * name-source: sibling-build
 *
 * Rebuilds the natives' land claims from scratch. Every tribe's four claim counters at +0x2e
 * are cleared, every tribe without bit 7 of +3 takes its turn (`indian_turn`), and then every colony
 * in the game re-stakes the squares of its own ring.
 *
 * A SQUARE IS RESTAKED ONLY IF ALL FOUR TESTS PASS: the colony must actually work it (its slot
 * is not negative), the square's plane-2 nibble must already be 4 or more -- a tribe and not
 * terrain -- it must not already belong to this colony's nation, and no European nation may
 * hold it. Anything else is left alone, so the pass only ever moves a square from a native
 * owner to the colony's own nation.
 *
 * The ring's size comes from `colony_map_status` through the one-based byte table at
 * DGROUP:0xc01, which `eligible_to_build` reads the same way.
 *
 * Both DGROUP:0x4b9e and DGROUP:0xc01 are declared FAR here: the shipped loads SEG34 into ES
 * for each of them, where the declaration the rest of the corpus uses reads them straight
 * off DS. `(int)slot < 0` needs its cast, or the byte is tested in place (rule 51).
 *
 * RENAMED 2026-09-26 from reset_tribe_claims: the Mac build's indian_turns(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee indian_turn; callee is_anything; callee owner_of; callee set_colony), the runner-up gets 1, and the arity agrees (0).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);
extern int colony_map_status(void);
extern char owner_of(int x, int y);
extern int is_anything(int x, int y);
extern void owner_set(int x, int y, int n);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void indian_turn(int tribe);

void indian_turns(void)
{
    int idx;
    int nation;
    int n;
    int cx;
    int cy;
    int x;
    int y;
    register int j;
    register int ring;

    for (nation = 0; nation < 8; nation++)
        for (j = 0; j < 4; j++)
            g_tribes[nation].mood[j] = 0;
    for (idx = 0; idx < 8; idx++)
        if (!(g_tribes[idx].flags & 0x80))
            indian_turn(idx);
    for (idx = 0; g_game.colony_count > idx; idx++) {
        set_colony(idx);
        nation = g_cur_colony_rec->nation;
        cx = g_cur_colony_rec->x;
        cy = g_cur_colony_rec->y;
        n = g_ring_tile_count[colony_map_status() - 1];
        for (ring = 0; ring < n; ring++) {
            x = g_ring_dx[ring] + cx;
            y = g_ring_dy[ring] + cy;
            if ((int)g_cur_colony_rec->slots[ring] < 0)
                continue;
            j = owner_of(x, y);
            if (j < 4)
                continue;
            if (j == nation)
                continue;
            if (is_anything(x, y) >= 0)
                continue;
            owner_set(x, y, nation);
        }
    }
}
