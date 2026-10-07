/* indian_village: candidate for 1030:9cd0 (981 bytes).  Verdict: RELOCATABLE.
 * symbol: indian_village
 * name-source: sibling-build
 *
 * One native settlement's turn (its record at DGROUP:0x4c2c, its tribe's at 0x4c28).
 *
 * GROWTH AND BRAVES. The growth byte (+6) gains the settlement's size (+4) each turn it grows -- when it
 * is below its initial size, or has flag bit 0 set -- and at 20 resets. A settlement below its initial
 * size then grows by one; otherwise it trains a brave: UNIT_BRAVES, one type higher (ARMED BRAVES) when
 * the tribe has muskets (+7, used up one in `difficulty + 1` times), two higher (MOUNTED) when it has 50
 * horses (+0xa, always used up) -- so both make MOUNTED WARRIORS. The brave is homed on the settlement
 * (Unit+6) and flag bit 0 cleared.
 *
 * THE CALM/ANGER BALANCE. The tribe keeps a signed byte per European nation (+0x36) that is converted
 * into real tension eight points at a time: each +8 calls `piss_off` with -1, lowering
 * tension, and each -8 with +1, raising it. Into it go:
 *   - for every nation the tribe is at peace with (pair flag 0x20), `q + 1` rolls against `12 - q`, where
 *     q is the square of the current tension's quarter -- the more tense, the more calming rolls, and
 *     the likelier each is;
 *   - for the nation whose MISSION is here (low nibble of +5): 1, or 4 with bit 0x10, doubled again
 *     when bit 4 of the flags is set; doubled by nation flag 0x18 and halved by 0x17 -- and the
 *     settlement's own anger for that nation (+0xa) drops by three times as much;
 *   - for the nation `village_anger` reports nearby: minus its count (same doubling), with the anger
 *     raised by that (halved if it is also the mission's nation) plus a fifth of the current tension.
 * Every remaining +8 for every nation is then spent at the end of the turn, while flag bit 0 of
 * SEG20:0x7784 is clear for the calming rolls.
 *
 * Five `(int)` casts on signed bytes (rule 51). `count >>= 1` shifts the variable in memory where
 * `sub = sub >> 1` shifts it in AX, and the shipped code has one of each.
 *
 * RENAMED 2026-09-26 from settlement_turn: the Mac build's indian_village(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (callee get_treaty; callee has_father; callee pissed; callee village_anger), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "settle.h"
#include "gameflag.h"
#include "units.h"
#include "nations.h"
#include "fathers.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_village(int s);

extern int pissed(int tribe, int i);
extern int piss_factor(int tension);
extern int village_anger(int s, int *count);
extern int has_father(int nation, int bit);
extern void piss_off(int tribe, int nation, int delta, int unused);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int base_defenders(int i);
extern int get_treaty(int nation_a, int nation_b);

void indian_village(int s)
{
    int grow = 0;
    int u;
    int nat;
    int type;
    int strength;
    int shift;
    int count;
    int sub;
    int roll;
    int tension;
    register int i;
    register int q;

    set_village(s);
    if ((unsigned char)base_defenders(s) > g_cur_settlement_rec->size)
        grow = 2;
    if (g_cur_settlement_rec->flags & 1)
        grow = 1;
    if (grow != 0) {
        g_cur_settlement_rec->growth += g_cur_settlement_rec->size;
        if ((int)g_cur_settlement_rec->growth >= 0x14) {
            g_cur_settlement_rec->growth = 0;
            if (grow == 2)
                g_cur_settlement_rec->size++;
            else {
                type = UNIT_BRAVES;
                if ((int)g_cur_tribe_rec->muskets > 0) {
                    if (rand_range(0, g_game.difficulty) == 0)
                        g_cur_tribe_rec->muskets--;
                    type++;
                }
                if (g_cur_tribe_rec->horses >= 0x32) {
                    g_cur_tribe_rec->horses -= 0x32;
                    type += 2;
                }
                u = create_unit(type, g_cur_settlement_rec->owner, g_cur_settlement_rec->x,
                                          g_cur_settlement_rec->y);
                if (u >= 0) {
                    g_units[u].home = s;
                    g_cur_settlement_rec->flags &= 0xfe;
                }
            }
        }
    }
    if (!(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
        for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
            if (get_treaty(i, g_cur_tribe_nation) & 0x20) {
                tension = pissed(g_cur_tribe, i);
                q = piss_factor(tension);
                q = q * q;
                count = 0;
                for (roll = 0; q + 1 > roll; roll++)
                    if (rand_range(0, 12 - q) == 0)
                        count++;
                g_cur_tribe_rec->calm[i] += count;
            }
        }
    }
    i = village_anger(g_cur_settlement, &strength);
    nat = g_cur_settlement_rec->mission;
    if (nat >= 0)
        nat &= 0xf;
    if (nat >= 0 || i >= 0) {
        shift = (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL) ? 1 : 0;
        if (nat >= 0) {
            count = ((g_cur_settlement_rec->mission & MISSION_EXPERT) ? 4 : 1) << shift;
            if (has_father(nat, FATHER_BARTOLOME_DE_LAS_CASAS))
                count <<= 1;
            if (has_father(nat, FATHER_JUAN_DE_SEPULVEDA))
                count >>= 1;
            g_cur_tribe_rec->calm[nat] += count;
            g_cur_settlement_rec->anger[nat] -= count * 2 + count;
            g_cur_settlement_rec->anger[nat] = max_int_1030_ead6(0, g_cur_settlement_rec->anger[nat]);
        }
        if (i >= 0) {
            sub = strength << shift;
            g_cur_tribe_rec->calm[i] -= sub;
            if (i == nat)
                sub = sub >> 1;
            sub += pissed(g_cur_tribe, i) / 5;
            g_cur_settlement_rec->anger[i] += sub;
        }
        if (nat >= 0) {
            while ((int)g_cur_tribe_rec->calm[nat] >= 8) {
                g_cur_tribe_rec->calm[nat] += -8;
                piss_off(g_cur_tribe, nat, -1, 3);
            }
        }
        if (i >= 0) {
            while ((int)g_cur_tribe_rec->calm[i] <= -8) {
                g_cur_tribe_rec->calm[i] += 8;
                piss_off(g_cur_tribe, i, 1, 5);
            }
        }
    }
    for (i = 0; i < NATION_EUROPEAN_COUNT; i++) {
        while ((int)g_cur_tribe_rec->calm[i] >= 8) {
            g_cur_tribe_rec->calm[i] += -8;
            piss_off(g_cur_tribe, i, -1, 0);
        }
    }
}
