/* colony_depletion: candidate for 1018:903e (217 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the colony's worked cells and exhausts any mine being worked. A cell with a
 * colonist whose job is 6 or 7 -- the two mining jobs -- and whose tile decoration is 6 or
 * 0xc gets bit 4 set in map plane 1, and `DEPLETION` is announced with the tile's
 * coordinates.
 *
 * The two loop bounds are spelled differently and that is the shipped source, not a
 * transcription slip: the outer is `row < 5` (`CMP 5 / JL`) and the inner is `col <= 5`
 * (`CMP 5 / JNG`). Borland compiles `< 6` and `<= 5` to different instructions, so the
 * asymmetry is visible and had to be written the way it reads.
 *
 * The tile is `colony->x + col - 2, colony->y + row - 2`, so the window is offset by two in
 * each direction from the colony square.
 *
 * Every one of the three tests is a POSITIVE `if` with a body, nested three deep -- not
 * three `continue`s. The shipped code jumps DIRECTLY past each body; a `continue` gives
 * Borland's invert-and-jump-over-a-jump (rule 12), which is what the first compile produced
 * and what the first divergence showed at +0x2f. The two spellings are indistinguishable in
 * C and four bytes apart in the object.
 *
 * `possible_popup` is the same seven-argument announcer produce_something uses, and the
 * flag byte at DGROUP:0x4bab is threaded through it the same way: passed in negated as the
 * "not yet shown" test and ORed back out of the return.
 *
 * RENAMED 2026-09-26 from deplete_mine: the Mac build's colony_depletion(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller new_turn_colony; callee get_job; callee possible_popup; callee resource_at), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "terrain.h"
#include "jobs.h"
#include "records.h"

extern int resource_at(int x, int y);
extern void feature_set(int x, int y, int bits, int on);

extern char get_field(int x, int y);
extern int get_job(int colonist);
extern char possible_popup(char *msg, int a, int b, int c, int d, int e, int f);

void colony_depletion(void)
{
    int x;
    int colonist;
    int row;
    int y;
    int deco;
    int job;
    register int col;

    for (row = 0; row < 5; row++) {
        for (col = 0; col <= 5; col++) {
            colonist = get_field(col, row);
            if (colonist >= 0) {
                job = get_job(colonist);
                if (job == JOB_SILVER_MINER || job == JOB_ORE_MINER) {
                    x = g_cur_colony_rec->x + col - 2;
                    y = g_cur_colony_rec->y + row - 2;
                    deco = resource_at(x, y);
                    if (deco == 0xc || deco == 6) {
                        feature_set(x, y, MAP1_DEPLETED, 1);
                        g_msg_seen |= possible_popup("DEPLETION", !g_msg_seen, -1, x, y, 3, 0);
                    }
                }
            }
        }
    }
}
