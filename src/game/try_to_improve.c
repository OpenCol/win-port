/* try_to_improve: candidate for 1020:3f1e (737 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The promotion roll for a soldier or dragoon (types 1 and 4) after a fight. A veteran (specialty
 * 0x15) is only considered once bit 0 of SEG20:0x7784 is set and the record of the nation at
 * SEG20:0x779a has bit 3; any other unit only when its specialty is not an ordinary job.
 * The chance is `base + bonus`, raised by the difficulty byte at SEG20:0x77a8 for a European nation
 * whose byte +0x31 is clear and lowered by it otherwise, less 10 for specialty 0x1a and 5 for 0x19.
 * Unless the nation has flag 0xb, `rand_range(1, chance)` must not exceed `bonus`.
 *
 * `next_improve` gives the specialty to promote to. None at all turns a European
 * soldier into type 9 and a dragoon into type 7 ("CONTINENTAL"); 0x15 is announced as "VETERAN";
 * anything else is "VALOR", naming the old and the new specialty. A non-European unit is promoted
 * without a message.
 *
 * The unit index is copied into a register local rather than enregistered as a parameter: SI
 * holds it and DI the chance, which only that spelling gives (rule 67).
 *
 * RENAMED 2026-09-26 from unit_promotion: the Mac build's try_to_improve(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller combat_fight; callee has_father; callee popm; callee popup_say), the runner-up gets 3, and the arity agrees (3). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "fathers.h"
#include "gameflag.h"
#include "jobs.h"
#include "units.h"
#include "records.h"

extern Nation g_nations[];             /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */
extern int any_spec(int job);

extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern int job_desc(int spec);

/* 5-byte NOP form, so extern (rule 47) */
extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int next_improve(int job);

void try_to_improve(int unit, int base, int bonus)
{
    register int u = unit;
    register int chance;
    int spec;
    int promo;

    if (g_units[u].type != UNIT_SOLDIERS && g_units[u].type != UNIT_DRAGOONS)
        goto out;
    if ((int)g_units[u].spec == JOB_SOLDIER) {
        if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0)
            goto out;
        if (!(g_nations[g_game.player_nation].flags & 8))
            goto out;
    } else {
        if (any_spec(g_units[u].spec))
            goto out;
    }
    chance = base + bonus;
    if ((unsigned char)(g_units[u].flags & 0xf) < 4
        && g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].control == 0)
        chance += g_game.difficulty;
    else
        chance -= g_game.difficulty;
    spec = g_units[u].spec;
    if (spec == JOB_CRIMINAL)
        chance -= 10;
    if (spec == JOB_INDENTURED_SERVANT)
        chance -= 5;
    if (has_father((unsigned char)(g_units[u].flags & 0xf), FATHER_GEORGE_WASHINGTON) == 0
        && rand_range(1, chance) > bonus)
        goto out;
    promo = next_improve(g_units[u].spec);
    if (spec == promo)
        goto out;
    if (promo < 0) {
        if ((unsigned char)(g_units[u].flags & 0xf) >= 4
            || g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].control != 0)
            goto out;
        if (g_units[u].type == UNIT_SOLDIERS)
            g_units[u].type = UNIT_CONTINENTAL_ARMY;
        else
            g_units[u].type = UNIT_CONTINENTAL_CAVALRY;
    } else {
        g_units[u].spec = promo;
    }
    if ((unsigned char)(g_units[u].flags & 0xf) < 4
        && g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].control == 0) {
        popup_say(0, g_unit_types[g_units[u].type].name);
        if (promo < 0) {
            popm("CONTINENTAL", 1);
        } else if (promo == 0x15) {
            popm("VETERAN", 1);
        } else {
            popup_say(1, job_desc(spec));
            popup_say(2, job_desc(promo));
            popm("VALOR", 1);
        }
    }
out:
    ;
}
