/* update_rebel_sentiment -- shipped at 1038:79e8.
 *
 * Ported by tools/port.py from matched/game/update_rebel_sentiment.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation2 far g_nations2[];                     SEG20:0xc2c4
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_rebel_sentiment;                    SEG20:0x77d2
 *   extern int far g_rebel_step;                         SEG20:0x77da
 *   extern unsigned char far g_nation_population[];      DGROUP:0x4b54
 *
 * The file's own Nation2 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 nation_sol_average(col_i16 nation);
void independence_war_turn(col_i16 nation);
void king_mobilises_forces(col_i16 nation);
void set_text_arg_i_1038_86cc(col_i16 slot, col_i16 v);
void spanish_succession_event(void);

#include "gameflag.h"
/* 0x13c bytes based at SEG20:0xc2c4, which is the nation array 0x30 BACK, so this
      struct's +0x30 is the canonical record's +0 and its +0x49 is the canonical +0x19.
      It is a separate typedef name for exactly that reason: its offsets are not the
      other Nation's and must not be read against them. */









/* same object: 4-byte `push cs; call ` (rule 47) */


extern void music_set_mood(col_i16 kind);

extern void set_nation_name_slot(col_i16 slot, col_i16 flag, col_i16 nation);
extern void text_message_show(char *msg, col_i16 n);

void update_rebel_sentiment(col_i16 nation)
{
    col_i16 s;
    col_i16 player;

    player = g_player_nation == nation || ((g_game_flags & GAMEFLAG_INDEPENDENCE) && g_mother_country_idx == nation);
    if (g_game_flags & GAMEFLAG_INDEPENDENCE) {
        if (player)
            independence_war_turn(nation);
        goto done;
    }
    if (player)
        king_mobilises_forces(nation);
    s = nation_sol_average(nation);
    g_nations2[nation].rebel = s;
    if (!player)
        goto done;
    g_rebel_sentiment = s;
    if (g_rebel_sentiment >= 0x32 && g_mother_country_idx < 0)
        spanish_succession_event();
    if (g_nation_population[nation] >= 4) {
        if (g_rebel_sentiment / 10 > g_rebel_step) {
            music_set_mood(3);
            set_text_arg_i_1038_86cc(0, g_rebel_sentiment);
            set_nation_name_slot(0, 0, g_player_nation);
            if (g_rebel_sentiment >= 0x32)
                text_message_show("REBELUP50", 1);
            else
                text_message_show("REBELUP", 1);
            g_rebel_step = g_rebel_sentiment / 10;
        } else if ((g_rebel_sentiment + 4) / 10 < g_rebel_step) {
            music_set_mood(1);
            set_text_arg_i_1038_86cc(0, g_rebel_sentiment);
            set_nation_name_slot(0, 0, g_player_nation);
            text_message_show("REBELDOWN", 1);
            g_rebel_step = g_rebel_sentiment / 10;
        }
    }
done:
    ;
}
