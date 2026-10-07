/* kill_tribe: candidate for 1030:848a (78 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Deletes every native settlement whose tribe byte (+2 of the 0x12-byte records) is
 * tribe + 4, walking the table backwards.
 *
 * RENAMED 2026-09-26 from destroy_tribe_settlements: the Mac build's kill_tribe(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between delete_village and indian_move, which are already bound; the arity agrees and the Mac body is 1.03x this one.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void delete_village(int i);
void kill_tribe(int tribe)
{
    int nation = tribe + 4;
    int i;

    for (i = g_game.settlement_count - 1; i >= 0; i--)
        if (g_settlements[i].owner == (unsigned char)nation)
            delete_village(i);
}
