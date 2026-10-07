/* tribe_heresy: candidate for 1030:a982 (760 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A missionary of nation `nat` denounces the mission in the current native settlement as heresy
 * ("HERESY0" / "HERESY1"). `owner` is the nation whose mission is there now (the low nibble of the
 * settlement's byte +5). Two totals and two alarms are built, A for `owner` and B for `nat`:
 *
 *   - over every settlement of the same tribe, the pull `village_anger` (1038:06b6) scores for its
 *     strongest European nation goes to A when that nation is `owner`, to B when it is `nat`; and a settlement with a mission adds its byte +4 -- doubled
 *     for bit 0x10 of +5 and again for bit 2 of +3 -- to B when `nat` is the owner and otherwise to
 *     A (the middle test compares `nat` with a copy of itself, so the third arm never runs);
 *   - A adds the tribe's table-78 word for `owner` shifted left by `flags & 4` of the current
 *     settlement's +3 -- 0 or 4, the masked value and not 1 -- B the word for `nat` shifted right
 *     by 1 minus that value (1, or -3 for a capital);
 *   - each alarm gains `piss_factor` of its own total (0..3, one step per 25) plus one; that bit also adds a d20 to both totals
 *     and doubles both alarms; a missionary of specialty 3 doubles A and its alarm, and bit 0x10 of
 *     the current settlement doubles B and its alarm.
 *
 * With the three nation names in text slots 0..2, `rand_range(1, A + B)` at or below A plays
 * sound 0x8024, shows "HERESY0", gives the settlement `nat`'s mission (with bit 0x10 for a
 * specialty-3 missionary) and negates alarm B; otherwise sound 0x53, "HERESY1", and alarm A is
 * negated. The missionary is removed and both alarms applied to the tribe through 1038:02a1.
 *
 * RENAMED 2026-09-26 from missionary_denounce: the Mac build's tribe_heresy(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller tribe_village; callee delete_unit; callee piss_factor; callee pissed; callee popm), the runner-up gets 5, and the arity agrees (3).
 */

#include "colonization.h"

#include "settle.h"
#include "jobs.h"
#include "sounds.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_village(int s);
extern int village_anger(int s, int *count);
extern int pissed(int tribe, int i);
extern int piss_factor(int tension);

extern void popup_say(int slot, int word);
extern void sound_play(int n);
extern int popm(char *msg, int n);
extern void delete_unit(int u);
extern void piss_off(int tribe, int nation, int alarm, int a);

/* same object: 4-byte `push cs; call ` (rule 47) */

void tribe_heresy(int u, int nat, unsigned char tribe)
{
    int me;
    int owner;
    int total_b;
    int jesuit;
    int jesuit_there;
    int alarm_b;
    int alarm_a;
    int i;
    int saved;
    int nib;
    int jes;
    int side;
    int count;
    int capital;
    register int v;
    register int total_a;

    me = nat;
    owner = g_cur_settlement_rec->mission & MISSION_NATION_MASK;
    total_a = total_b = 0;
    alarm_b = alarm_a = 0;
    saved = g_cur_settlement;
    for (i = 0; g_game.settlement_count > i; i++) {
        set_village(i);
        if (g_cur_settlement_rec->owner != tribe)
            continue;
        v = 0;
        side = village_anger(i, &count);
        if (side == owner)
            total_a += count;
        else if (side == me)
            total_b += count;
        else
            v += count;
        if ((int)g_cur_settlement_rec->mission >= 0) {
            nib = g_cur_settlement_rec->mission & MISSION_NATION_MASK;
            jes = g_cur_settlement_rec->mission & MISSION_EXPERT;
            v += g_cur_settlement_rec->size;
            if (jes)
                v = v * 2;
            if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
                v = v * 2;
            if (nat == owner)
                total_b += v;
            else if (nat == me)
                total_a += v;
            else {
                alarm_b += v;
                alarm_a += v;
            }
        }
    }
    set_village(saved);
    jesuit = (int)g_units[u].spec == JOB_COTTON_PLANTER;
    jesuit_there = g_cur_settlement_rec->mission & MISSION_EXPERT;
    capital = g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL;
    total_a += pissed(g_cur_tribe, owner) << capital;
    total_b += pissed(g_cur_tribe, me) >> (1 - capital);
    alarm_b += piss_factor(total_b) + 1;
    alarm_a += piss_factor(total_a) + 1;
    if (capital != 0) {
        total_a += rand_range(1, 20);
        total_b += rand_range(1, 20);
        alarm_a = alarm_a * 2;
        alarm_b = alarm_b * 2;
    }
    if (jesuit != 0) {
        total_a = total_a * 2;
        alarm_a = alarm_a * 2;
    }
    if (jesuit_there != 0) {
        total_b = total_b * 2;
        alarm_b = alarm_b * 2;
    }
    popup_say(0, get_adjective(me));
    popup_say(1, get_adjective(owner));
    popup_say(2, get_adjective(g_cur_tribe_nation));
    if (rand_range(1, total_a + total_b) <= total_a) {
        sound_play(SOUND_AMEN);
        popm("HERESY0", 4);
        g_cur_settlement_rec->mission = me;
        if (jesuit != 0)
            g_cur_settlement_rec->mission |= MISSION_EXPERT;
        alarm_b *= -1;
    } else {
        sound_play(SOUND_FIRE1);
        popm("HERESY1", 4);
        alarm_a *= -1;
    }
    delete_unit(u);
    piss_off(g_cur_tribe, owner, alarm_a, 0);
    piss_off(g_cur_tribe, me, alarm_b, 0);
}
