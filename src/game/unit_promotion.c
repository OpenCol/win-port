/* unit_promotion -- shipped at 1020:3f1e.
 *
 * Ported by tools/port.py from matched/game/unit_promotion.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern Nation34 far g_nation_text[];                 SEG20:0x7810
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Unit, UnitType, Nation, Nation34 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 map_job_overrides(col_i16 job);

#include "fathers.h"
#include "gameflag.h"
#include "jobs.h"
#include "units.h"
extern col_i16 is_ordinary_job(col_i16 job);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void text_message_show(char *msg, col_i16 n);
extern col_i16 spec_expert_name(col_i16 spec);

/* 5-byte NOP form, so extern (rule 47) */
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);

/* same object: 4-byte `push cs; call ` (rule 47) */


void unit_promotion(col_i16 unit, col_i16 base, col_i16 bonus)
{
    register col_i16 u = unit;
    register col_i16 chance;
    col_i16 spec;
    col_i16 promo;

    if (g_units[u].type != UNIT_SOLDIERS && g_units[u].type != UNIT_DRAGOONS)
        goto out;
    if ((col_i16)g_units[u].spec == JOB_SOLDIER) {
        if ((g_game_flags & GAMEFLAG_INDEPENDENCE) == 0)
            goto out;
        if (!(g_nations[g_player_nation].flags & 8))
            goto out;
    } else {
        if (is_ordinary_job(g_units[u].spec))
            goto out;
    }
    chance = base + bonus;
    if ((unsigned char)(g_units[u].flags & 0xf) < 4
        && g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].met == 0)
        chance += g_difficulty;
    else
        chance -= g_difficulty;
    spec = g_units[u].spec;
    if (spec == 0x1a)
        chance -= 10;
    if (spec == 0x19)
        chance -= 5;
    if (nation_flag_test((unsigned char)(g_units[u].flags & 0xf), FATHER_GEORGE_WASHINGTON) == 0
        && rand_range(1, chance) > bonus)
        goto out;
    promo = map_job_overrides(g_units[u].spec);
    if (spec == promo)
        goto out;
    if (promo < 0) {
        if ((unsigned char)(g_units[u].flags & 0xf) >= 4
            || g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].met != 0)
            goto out;
        if (g_units[u].type == UNIT_SOLDIERS)
            g_units[u].type = UNIT_CONTINENTAL_ARMY;
        else
            g_units[u].type = UNIT_CONTINENTAL_CAVALRY;
    } else {
        g_units[u].spec = promo;
    }
    if ((unsigned char)(g_units[u].flags & 0xf) < 4
        && g_nation_text[(unsigned char)(g_units[u].flags & 0xf)].met == 0) {
        set_name_slot_from_text(0, g_unit_types[g_units[u].type].name);
        if (promo < 0) {
            text_message_show("CONTINENTAL", 1);
        } else if (promo == 0x15) {
            text_message_show("VETERAN", 1);
        } else {
            set_name_slot_from_text(1, spec_expert_name(spec));
            set_name_slot_from_text(2, spec_expert_name(promo));
            text_message_show("VALOR", 1);
        }
    }
out:
    ;
}
