/* create_village: candidate for 1030:81f1 (255 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Adds a settlement of `nation` (a tribe, 4 and up) at (x, y) when fewer than 84 exist: the
 * record is made current, its tribe made the current tribe, position and tribe stored, the
 * byte +4 from base_defenders, no mission (+5 = -1), plane-1 bit 1 set and the owner nibble
 * in plane 2; bytes +3, +7, +8, +9 initialised. Returns the new index or -1.
 *
 * RENAMED 2026-09-26 from native_settlement_create: the Mac build's create_village(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee feature_loc; callee owner_set; callee set_tribe; callee set_village), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_village(int s);
extern void set_tribe(int tribe);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern char base_defenders(int s);
extern unsigned char *feature_loc(int x, int y);
extern void owner_set(int x, int y, int n);

int create_village(int nation, int x, int y)
{
    int s = -1;

    if (g_game.settlement_count >= 0x54)
        goto done;
    {
        s = g_game.settlement_count;
        g_game.settlement_count++;
        set_village(s);
        set_tribe(nation - 4);
        g_cur_settlement_rec->owner = nation;
        g_cur_settlement_rec->x = x;
        g_cur_settlement_rec->y = y;
        g_cur_settlement_rec->size = base_defenders(s);
        g_cur_settlement_rec->mission = 0xff;
        g_cur_settlement_rec->growth = 0;
        *feature_loc(x, y) |= 2;
        owner_set(x, y, nation);
        g_settlements[s].flags = 0;
        g_settlements[s].refused = 0xff;
        g_settlements[s].sold = 0xff;
        g_settlements[s].bought = 0xff;
    }
done:
    return s;
}
