/* revolution: candidate for 1038:6aad (1402 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE DECLARATION OF INDEPENDENCE, run from the menu through `liberty_declared` (1038:7bc2).
 *
 * THE CROWN NEEDS A SLOT. If SEG20:0x77d4 is still negative, the War of Spanish Succession is
 * fought first (`conquest`), and the power it removes leaves its slot there.
 * That slot becomes the Crown: its control byte is set to 1 (computer), it is put at war with the
 * player (pair flags 0x22 set, 0x40 cleared), and every map square the player has claimed (plane-3
 * bit 0x10 << player) is claimed for it as well. So 0x77d4 is the eliminated power until this
 * moment, and the King's nation from now on.
 *
 * THE OTHER TWO POWERS are ranked with the same score the succession uses, lowest first. The first that is
 * neither the player nor the Crown becomes the INTERVENING power (SEG20:0x77d6) if none is set yet,
 * and the next one is stored at 0x77d8. Its expeditionary force is sized in 0x77e4..0x77ea from the
 * intervening power's strengths and eased by difficulty, each halved (rounding up):
 *   - 0x77e8, the ships: two bytes of its 19-byte record, plus `(4 - difficulty) / 2 + 3`;
 *   - 0x77ea: its word at 0xd9d6 / 32, plus the same `(4 - difficulty) / 2 + 3`, at most twice the ships;
 *   - 0x77e6: its byte at DGROUP:0x4b6c / 16, plus `(4 - difficulty) / 2 + 1`, at most twice the ships;
 *   - 0x77e4: its byte at DGROUP:0x4b54 / 10, plus `8 - difficulty`, and at most six per ship less
 *     the other two.
 * `revolution_check` lands this force once intervention is declared, and only if 0x77e8 is non-zero.
 *
 * AND THE REST: the year is split into century and year bytes (0x77a9, 0x77aa); every other power
 * is marked defeated (`intern`); the player's ships on the high seas are dealt
 * with; bit 0 of 0x7784 records the revolution; the player's units are re-rated. The field +0x16
 * of both nation records is cleared, and bit 2 of the Crown's flags. INDEPENDENCE is shown.
 *
 * A LIKELY SHIPPED SLIP: before copying the claims, each plane-3 byte is ANDed with `~crown`,
 * the nation NUMBER, where `~(0x10 << crown)` would clear the Crown's own claim bit. As shipped it
 * clears one of bits 0..3 instead (none for England, whose number is 0).
 *
 * The one DIFFERS was a grouping: `(fleet + 1 >> 5) + ((4 - difficulty) / 2 + 3)`, where the shipped
 * adds the constant into the difficulty term before the sum.
 *
 * THE FOUR COUNTS ARE ONE ARRAY. This file used to declare SEG20:0x77e4, 0x77e6, 0x77e8 and
 * 0x77ea as four separate `int ` globals, two of them address-named. They are `g_game.force[0..3]`
 * -- `include/royal.h`'s four kinds -- which `start_new_game`, `ReportWin_ReportsContinental` and
 * `allied_invasion` all declare as one array and index with one `kind`.
 *
 * Written out that way, the last line is the fifth independent thing to say which index is the
 * fleet: `[0] = min([0], [2] * 6 - ([3] + [1]))` caps the land force at six per ship.
 * `royal.h` had the other four.
 *
 * RENAMED 2026-09-27 from declaration_of_independence: String witness: INDEPENDENCE is in Mac revolution(void). Call graph: Mac revolution's callers are exactly MainWin::MainMenuProc and liberty_declared, and 6aad's callers are exactly MainWin_MainMenuProc and liberty_declared. Mac revolution calls set_europe, conquest, force_mood, declaration_of_independence, sort_16x16, intern, kill_offboard, revolution_setup, site_loc, treaty_on, treaty_off, popm and end_unit_turn. The reconstruction calls the same list in the same order, with liberty_spell_name (1038:660e) where the Mac build calls declaration_of_independence. The Windows-only extras are DrawMap and min_int. Arity: (void) on both sides. Size is 1136 Mac against 1402 Windows. Layout: Mac CODE 22 has declaration_of_independence (0x5410) before revolution (0x5cd6), and Windows has 660e before 6aad. Why it went wrong: the INDEPENDENCE text is 'Continental Congress signs Declaration of Independence!', which produced the paraphrase sign_declaration_of_independence. tools/mac_rename.py then stripped the paraphrase to the Mac identifier, and its 0-0 arity check could not tell the two void functions apart.
 */

#include "colonization.h"

#include "royal.h"
#include "control.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern Nation g_nations[];              /* SEG20:0xc2f4, 316 bytes each */

extern GameState g_game;   /* SEG20:0x7782 */
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern void set_europe(int n);
extern void force_mood(int kind);
extern void sort_16x16(int n, int *payload, int *key);
extern unsigned char * site_loc(int x, int y);
extern int treaty_on(int nation, int other, int mask);
extern int treaty_off(int nation, int other, int mask);
extern void DrawMap(int full);
extern int popm(char *msg, int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void conquest(void);

extern void intern(int nation);
extern void kill_offboard(int nation);
extern void revolution_setup(void);

extern void end_unit_turn(int u);

void revolution(void)
{
    int i;
    int pbit;
    int cbit;
    int cmask;
    int y;
    int player;
    int ships;
    int score[4];
    int ids[4];
    unsigned char *p3;
    register int x;
    register int u;

    player = g_game.player_nation;
    set_europe(player);
    g_game.year_lo = g_game.year % 100;
    g_game.year_hi = g_game.year / 100;
    g_cur_nation_rec->bells = 0;
    if (g_game.mother_country_idx < 0)
        conquest();
    force_mood(3);
    declaration_of_independence();
    for (i = 0; i < 4; i++) {
        ids[i] = (unsigned char)i;
        score[i] = g_nation_population[i] + g_nation_colony_count[i] * 2 + g_nation_ship_count[i] * 3;
    }
    sort_16x16(4, ids, score);
    g_game.merc_nation = -1;
    for (i = 0; g_game.merc_nation < 0 && i < 4; i++) {
        if (ids[i] == (unsigned char)g_game.player_nation)
            continue;
        if (ids[i] == (unsigned char)g_game.mother_country_idx)
            continue;
        if (g_game.intervention_nation < 0)
            g_game.intervention_nation = ids[i];
        else
            g_game.merc_nation = ids[i];
    }
    ships = g_unit_tally[g_game.intervention_nation][UNIT_PRIVATEER]
            + g_unit_tally[g_game.intervention_nation][UNIT_FRIGATE];
    g_game.force[ROYAL_REGULARS] = 4 - g_game.difficulty + g_nation_population[g_game.intervention_nation] / 10 + 4;
    g_game.force[ROYAL_CAVALRY] = ((4 - g_game.difficulty) >> 1) + ((g_nation_strength[g_game.intervention_nation] + 1) >> 4) + 1;
    g_game.force[ROYAL_ARTILLERY] = ((g_military_power[g_game.intervention_nation] + 1) >> 5) + (((4 - g_game.difficulty) >> 1) + 3);
    g_game.force[ROYAL_MAN_O_WAR] = ((4 - g_game.difficulty) >> 1) + ships + 3;
    g_game.force[ROYAL_REGULARS] = (g_game.force[ROYAL_REGULARS] + 1) / 2;
    g_game.force[ROYAL_CAVALRY] = (g_game.force[ROYAL_CAVALRY] + 1) / 2;
    g_game.force[ROYAL_ARTILLERY] = (g_game.force[ROYAL_ARTILLERY] + 1) / 2;
    g_game.force[ROYAL_MAN_O_WAR] = (g_game.force[ROYAL_MAN_O_WAR] + 1) / 2;
    g_game.force[ROYAL_ARTILLERY] = min_int_1038_8680(g_game.force[ROYAL_ARTILLERY], g_game.force[ROYAL_MAN_O_WAR] * 2);
    g_game.force[ROYAL_CAVALRY] = min_int_1038_8680(g_game.force[ROYAL_CAVALRY], g_game.force[ROYAL_MAN_O_WAR] * 2);
    g_game.force[ROYAL_REGULARS] = min_int_1038_8680(g_game.force[ROYAL_REGULARS], g_game.force[ROYAL_MAN_O_WAR] * 6 - (g_game.force[ROYAL_ARTILLERY] + g_game.force[ROYAL_CAVALRY]));
    for (i = 0; i < 4; i++)
        if (g_game.player_nation != i && g_game.mother_country_idx != i)
            intern(i);
    kill_offboard(g_game.player_nation);
    revolution_setup();
    g_game.game_flags |= 1;
    p3 = site_loc(0, 0);
    pbit = 0x10 << g_game.player_nation;
    cbit = 0x10 << g_game.mother_country_idx;
    cmask = ~g_game.mother_country_idx;
    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++) {
            *p3 &= (unsigned char)cmask;
            if (*p3 & pbit)
                *p3 |= (unsigned char)cbit;
            p3++;
        }
    }
    g_nation_text[g_game.mother_country_idx].control = CONTROL_AI;
    treaty_on(player, g_game.mother_country_idx, 0x22);
    treaty_off(player, g_game.mother_country_idx, 0x40);
    DrawMap(1);
    set_name_slot_1038_8775(0, g_nation_text[player].leader);
    popm("INDEPENDENCE", 1);
    for (u = g_game.unit_count - 1; u >= 0; u--)
        if (NATION(g_units[u]) == (unsigned char)player)
            end_unit_turn(u);
    g_nations[player].f16 = 0;
    g_nations[g_game.mother_country_idx].f16 = 0;
    g_nations[g_game.mother_country_idx].flags &= ~4;
}
