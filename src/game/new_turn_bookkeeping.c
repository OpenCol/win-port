/* new_turn_bookkeeping: candidate for 1020:830f (1235 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE START OF A EUROPEAN NATION'S TURN (the nation on turn, SEG20:0x7796), run by `1008:2be6`.
 *
 * UNITS, last to first, for the nation's own units:
 *   - each explores its square (`explore_square`), and a unit `unit_rules` removes
 *     goes no further;
 *   - a DAMAGED SHIP (bit 7 of +4) counts a repair turn in +0x16, two when it is on the map. At the
 *     type's refit turns (+5 of the unit-type record) the damage bit is cleared, and a human hears
 *     REFIT with the ship's type and the colony it is in, or the nation's name when it is in Europe.
 *     DGROUP:0xdc6 is set in that case so the Europe status is refreshed.
 *
 * THEN, in order: the nation's native-contact codes are tallied, the Europe turn runs
 * (`europe_new_turn`), the treasure fleet is checked, and the Europe status is refreshed if a
 * ship was refitted there. The market's +0xe is cleared, every colony of the nation takes its turn
 * (`new_turn_colony`), flag 10 is cleared, the nation is recounted, and rebel sentiment
 * is updated.
 *
 * THE KING'S FRIGATE. Every eighth turn, before the revolution, a nation with none of the counted
 * frigates (`g_unit_tally[nation][UNIT_FRIGATE]` is 0) is offered a FRIGATE, provided DGROUP
 * 0x4c0b is set or 0x4c0c is above 3. A human is asked with KINGFRIGATE; a computer nation accepts.
 * The frigate is created in Europe (x and y both `nation - 24`), with its destination at the market's
 * dock square (+0x32, +0x33), its arrival countdown started, and bit 6 of +4 set. A human then pays
 * for it in tax: KINGTAX through `raise_taxes` with 10.
 *
 * WHAT THE BYTES SETTLED: the damage and flag tests are `(x & bit) == 0` (tested in place; the `!`
 * spelling promotes the byte first), and the refit tail is nested `if`s rather than `continue`s
 * (386 long conditional jumps, rule 12). A local is set to 0 and never used.
 *
 * RENAMED 2026-09-26 from nation_turn: the Mac build's new_turn_bookkeeping(void), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DoViceroy; callee Show; callee new_turn_colony; callee on_map; callee popup_say), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "control.h"
#include "gameflag.h"
#include "units.h"
#include "sounds.h"
#include "nations.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

/* PORT: this file's own EuropeWin removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"

extern void bottom_box(int colour);

extern int unit_rules(int u);

extern void popup_say(int slot, int word);

extern void sound_play(int n);
extern int popm(char *msg, int n);
extern void collect_stats_1(int nation);
extern void europe_new_turn(int nation);
extern void process_off_board_ships(void);
extern void EuropeWin_Show(void *p, int nation, int unit);
extern void new_turn_colony(int colony);
extern void collect_stats(int nation);
extern void rebel_check(int nation);

extern void force_music(int what);

extern int get_voyage_delay(int u, int tile_x, int b);
extern void raise_taxes(char *key, int delta);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void assess_upkeep(void);

void new_turn_bookkeeping(void)
{
    int colony;
    int answer;
    int nation;
    int unused;
    register int u;

    unused = 0;
    nation = g_game.turn_nation;
    bottom_box(g_nation_colour[nation]);
    g_show_europe = 0;
    g_europe_arrival_unit = -1;
    for (u = g_game.unit_count - 1; u >= 0; u--) {
        if (NATION(g_units[u]) != (unsigned char)nation)
            continue;
        explore_square(u);
        if (!unit_rules(u))
            continue;
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            continue;
        if ((g_units[u].flags2 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
            continue;
        g_units[u].counter++;
        if (on_map(g_units[u].x, g_units[u].y))
            g_units[u].counter++;
        if (g_unit_types[g_units[u].type].defence - g_units[u].counter <= 0) {
            g_units[u].flags2 &= 0x7f;
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
                popup_say(0, g_unit_types[g_units[u].type].name);
                if (on_map(g_units[u].x, g_units[u].y)) {
                    colony = find_colony(g_units[u].x, g_units[u].y);
                    if (colony >= 0)
                        set_name_slot_1020_a94a(1, g_colonies[colony].name);
                } else {
                    popup_say(1, g_nation_name_word[nation]);
                    g_show_europe = 1;
                }
                sound_play(SOUND_HAMMERS);
                popm("REFIT", 0);
            }
        }
    }
    collect_stats_1(nation);
    europe_new_turn(nation);
    process_off_board_ships();
    if (g_show_europe != 0)
        EuropeWin_Show(&g_europe_win, nation, g_europe_arrival_unit);
    g_cur_nation_rec->bells_total = 0;
    for (colony = g_game.colony_count - 1; colony >= 0; colony--)
        if (g_colonies[colony].nation == (unsigned char)nation)
            new_turn_colony(colony);
    assess_upkeep();
    collect_stats(nation);
    rebel_check(nation);
    if ((g_colonies_blockaded != 0 || g_colonies_besieged > 3) && g_unit_tally[nation][UNIT_FRIGATE] == 0 && (g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0
        && (g_game.turn_counter & 7) == 0) {
        if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
            popup_say(0, g_difficulty_words[g_game.difficulty]);
            set_name_slot_1020_a94a(1, g_nation_text[nation].leader);
            popup_say(2, get_adjective(nation));
            force_music(SOUND_MEETKING);
            answer = show_text_dialog_1020_aa02("KINGFRIGATE");
        } else
            answer = 1;
        if (answer == 1) {
            u = create_unit(UNIT_FRIGATE, nation, nation - 0x18, nation - 0x18);
            if (u < 0)
                return;
            g_units[u].orders = ORDERS_NONE;
            g_units[u].dest_x = g_cur_nation_rec->start_x;
            g_units[u].dest_y = g_cur_nation_rec->start_y;
            g_units[u].counter = get_voyage_delay(u, g_cur_nation_rec->start_x, g_cur_nation_rec->start_y);
            g_units[u].flags2 |= 0x40;
            if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
                raise_taxes("KINGTAX", 10);
        }
    }
}
