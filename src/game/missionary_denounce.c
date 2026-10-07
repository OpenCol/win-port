/* missionary_denounce -- shipped at 1030:a982.
 *
 * Ported by tools/port.py from matched/game/missionary_denounce.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far *g_cur_settlement_rec;         DGROUP:0x4c2c
 *   extern int g_cur_tribe;                              DGROUP:0x4c30
 *   extern int g_cur_tribe_nation;                       DGROUP:0x4c32
 *   extern int g_cur_settlement;                         DGROUP:0x4c34
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Settlement, Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 lookup_with_two_overrides(col_i16 n);

#include "settle.h"
#include "jobs.h"
#include "sounds.h"
extern void set_current_native_settlement(col_i16 s);
extern col_i16 settlement_influence(col_i16 s, col_i16 *count);
extern col_i16 table_78_word(col_i16 t, col_i16 i);
extern col_i16 quarter_of_100(col_i16 v);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void play_sound_effect(col_i16 n);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void unit_record_free(col_i16 u);
extern void tribe_hostile(col_i16 v, col_i16 nation, col_i16 alarm, col_i16 a);

/* same object: 4-byte `push cs; call ` (rule 47) */


void missionary_denounce(col_i16 u, col_i16 nat, unsigned char tribe)
{
    col_i16 me;
    col_i16 owner;
    col_i16 total_b;
    col_i16 jesuit;
    col_i16 jesuit_there;
    col_i16 alarm_b;
    col_i16 alarm_a;
    col_i16 i;
    col_i16 saved;
    col_i16 nib;
    col_i16 jes;
    col_i16 side;
    col_i16 count;
    col_i16 capital;
    register col_i16 v;
    register col_i16 total_a;

    me = nat;
    owner = g_cur_settlement_rec->mission & MISSION_NATION_MASK;
    total_a = total_b = 0;
    alarm_b = alarm_a = 0;
    saved = g_cur_settlement;
    for (i = 0; g_settlement_count > i; i++) {
        set_current_native_settlement(i);
        if (g_cur_settlement_rec->owner != tribe)
            continue;
        v = 0;
        side = settlement_influence(i, &count);
        if (side == owner)
            total_a += count;
        else if (side == me)
            total_b += count;
        else
            v += count;
        if ((col_i16)g_cur_settlement_rec->mission >= 0) {
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
    set_current_native_settlement(saved);
    jesuit = (col_i16)g_units[u].spec == JOB_COTTON_PLANTER;
    jesuit_there = g_cur_settlement_rec->mission & MISSION_EXPERT;
    capital = g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL;
    total_a += table_78_word(g_cur_tribe, owner) << capital;
    total_b += table_78_word(g_cur_tribe, me) >> (1 - capital);
    alarm_b += quarter_of_100(total_b) + 1;
    alarm_a += quarter_of_100(total_a) + 1;
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
    set_name_slot_from_text(0, lookup_with_two_overrides(me));
    set_name_slot_from_text(1, lookup_with_two_overrides(owner));
    set_name_slot_from_text(2, lookup_with_two_overrides(g_cur_tribe_nation));
    if (rand_range(1, total_a + total_b) <= total_a) {
        play_sound_effect(SOUND_AMEN);
        text_message_show("HERESY0", 4);
        g_cur_settlement_rec->mission = me;
        if (jesuit != 0)
            g_cur_settlement_rec->mission |= MISSION_EXPERT;
        alarm_b *= -1;
    } else {
        play_sound_effect(SOUND_FIRE1);
        text_message_show("HERESY1", 4);
        alarm_a *= -1;
    }
    unit_record_free(u);
    tribe_hostile(g_cur_tribe, owner, alarm_a, 0);
    tribe_hostile(g_cur_tribe, me, alarm_b, 0);
}
