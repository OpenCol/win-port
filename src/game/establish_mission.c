/* establish_mission -- shipped at 1030:a6bc.
 *
 * Ported by tools/port.py from matched/game/establish_mission.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far *g_cur_settlement_rec;         DGROUP:0x4c2c
 *   extern int g_cur_tribe;                              DGROUP:0x4c30
 *   extern int g_cur_settlement;                         DGROUP:0x4c34
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern int far g_mission_word[];                     SEG20:0xd8b6
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *   extern int far g_season;                             SEG20:0x778e
 *   extern int far g_season_word[];                      DGROUP:0x4b34
 *   extern int far g_year;                               SEG20:0x778c
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * The file's own Settlement, Unit, Colony, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 lookup_with_two_overrides(col_i16 n);
col_i16 max_int_1030_ead6(col_i16 a, col_i16 b);
void set_name_slot(col_i16 i, char *s);

#include "control.h"
#include "settle.h"
#include "nations.h"
#include "fathers.h"
#include "jobs.h"
#include "sounds.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */













extern void set_current_native_settlement(col_i16 s);
extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);
extern col_i16 table_78_word(col_i16 t, col_i16 i);
extern col_i16 quarter_of_100(col_i16 v);
extern char * strcpy(char *d, const char *s);
extern col_i16 find_nearest_colony(col_i16 x, col_i16 y, col_i16 nation, col_i16 land);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void play_sound_effect(col_i16 n);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void unit_record_free(col_i16 u);
extern void tribe_hostile(col_i16 tribe, col_i16 nation, col_i16 delta, col_i16 unused);
/* same object: 4-byte `push cs; call ` (rule 47) */




void establish_mission(col_i16 u, col_i16 nation, col_i16 named)
{
    col_i16 mood;
    col_i16 col;
    col_i16 s;
    col_i16 tribe;
    col_i16 saved;
    char msg[0x28];
    register col_i16 missions;
    register col_i16 alarm;

    missions = 0;
    alarm = 0;
    tribe = g_cur_tribe;
    saved = g_cur_settlement;
    for (s = 0; s < g_settlement_count; s++) {
        set_current_native_settlement(s);
        if (g_cur_tribe != tribe)
            continue;
        if (((col_i16)g_cur_settlement_rec->mission & MISSION_NATION_MASK) == nation)
            missions++;
    }
    set_current_native_settlement(saved);
    if (nation_flag_test(nation, FATHER_JUAN_DE_SEPULVEDA))
        missions += missions;
    if (nation_flag_test(nation, FATHER_BARTOLOME_DE_LAS_CASAS))
        missions >>= 1;
    if (nation_flag_test(nation, FATHER_POCAHONTAS))
        missions >>= 1;
    if (nation == NATION_FRANCE)
        missions >>= 1;
    mood = quarter_of_100(table_78_word(g_cur_tribe, nation));
    switch (mood) {
    case 0:
        alarm = (missions << 3) - 25;
        break;
    case 1:
        alarm = (missions << 3) - 15;
        break;
    case 2:
        alarm = (missions << 3) - 10;
        break;
    case 3:
    default:
        alarm = (missions << 3) - 5;
        break;
    }
    if (g_cur_settlement_rec->flags & SETTLEMENT_CAPITAL)
        alarm += (alarm > 0 ? alarm : ~alarm + 1) << 3;
    strcpy(msg, "MISSION0");
    if (alarm >= -5)
        mood = max_int_1030_ead6(1, mood);
    if (alarm > 0)
        mood = max_int_1030_ead6(2, mood);
    if (alarm >= 10)
        mood = 3;
    msg[7] += (char)mood;
    col = find_nearest_colony(g_cur_settlement_rec->x, g_cur_settlement_rec->y, nation, -1);
    set_name_slot_from_text(0, g_mission_word[nation]);
    if (col >= 0)
        set_name_slot(1, ((char *)((unsigned char *)&(g_colonies[col]) + 0x2)));
    else
        set_name_slot_from_text(1, g_nation_name_word[nation]);
    set_name_slot_from_text(2, g_season_word[g_season]);
    g_text_args[0] = g_year;
    set_name_slot_from_text(3, lookup_with_two_overrides(named));
    if (nation < NATION_EUROPEAN_COUNT && g_nation52[nation].control == CONTROL_HUMAN) {
        play_sound_effect(SOUND_AMEN);
        text_message_show(msg, 4);
    }
    g_cur_settlement_rec->mission = nation;
    if ((col_i16)g_units[u].spec == JOB_MISSIONARY || nation_flag_test(nation, FATHER_JEAN_DE_BREBEUF))
        g_cur_settlement_rec->mission |= MISSION_EXPERT;
    unit_record_free(u);
    tribe_hostile(g_cur_tribe, nation, alarm, 0);
}
