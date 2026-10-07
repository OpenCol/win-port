/* conquest: candidate for 1038:49d1 (1059 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE WAR OF SPANISH SUCCESSION: one European power leaves the game and another absorbs it.
 * `rebel_check` fires it once the player's rebels reach 50% while SEG20:0x77d4 is still
 * negative, and `revolution` (1038:6aad) calls it too.
 *
 * WHO. The four powers are scored `g_nation_population[n] + 2 * colonies[n] + 3 * g_nation_ship_count[n]` and sorted
 * ascending. The LOSER is the lowest-scoring power that is not the player. The WINNER is the
 * lowest-scoring power left that is neither the loser nor human. SUCCESSION names them.
 *
 * WHAT CHANGES HANDS:
 *   - on every map square, the loser's plane-3 bit (0x10 << nation) gains the winner's, and a
 *     plane-2 owner nibble of the loser becomes the winner;
 *   - every loser unit on the map becomes the winner's; units off the map (in Europe or at sea
 *     to it) are freed. The unit's whole flags byte is overwritten with the winner's number,
 *     clearing its upper bits too;
 *   - every loser colony becomes the winner's, and its long at +0xc2 is zeroed;
 *   - every native mission of the loser now belongs to the winner, keeping its expert bit.
 * Both nations are then recomputed (`collect_stats`), the loser's control byte becomes 2
 * (gone), and SEG20:0x77d4 takes the LOSER's number. That is the slot `revolution_check`
 * later compares against as the Crown's nation, so the name `g_game.mother_country_idx` may cover two
 * roles; this function only shows it holding the eliminated power.
 *
 * It compiled RELOCATABLE on the first build: the byte-promoted `(unsigned char)i` for the
 * sort keys and an `unsigned` key array (its `< 4` test is `JNC`), with plane pointers.
 *
 * RENAMED 2026-09-26 from spanish_succession_event: the Mac build's conquest(void), bound by call-graph agreement (tools/mac_callgraph.py): 11 of its Mac call edges already bound land on this function (caller MainMenuProc; callee collect_stats; callee delete_unit; callee force_mood; callee on_map), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "control.h"
#include "settle.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern void sort_16x16(int n, int *payload, int *key);
extern void force_mood(int kind);
extern void popup_country(int slot, int flag, int nation);
extern int get_name(int nation);
extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern unsigned char * site_loc(int x, int y);
extern unsigned char * continent_loc(int x, int y);

extern void owner_set(int x, int y, int nation);
extern void set_colony(int colony);
extern void collect_stats(int nation);
extern void DrawMap(int full);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void delete_unit(int u);
extern void set_village(int s);

void conquest(void)
{
    int loser;
    int winner;
    int lbit;
    int wbit;
    int i;
    int y;
    int score[4];
    unsigned ids[4];
    unsigned char *p3;
    unsigned char *p2;
    register int x;
    register int u;

    for (i = 0; i < 4; i++) {
        ids[i] = (unsigned char)i;
        score[i] = g_nation_population[i] + g_nation_colony_count[i] * 2 + g_nation_ship_count[i] * 3;
    }
    sort_16x16(4, (int *)ids, score);
    loser = winner = -1;
    for (i = 0; loser < 0 && i < 4; i++) {
        if (ids[i] == (unsigned char)g_game.player_nation)
            continue;
        loser = ids[i];
    }
    for (i = 0; winner < 0 && i < 4; i++) {
        if (ids[i] == (unsigned char)loser)
            continue;
        if (ids[i] < 4 && g_nation_text[ids[i]].control == CONTROL_HUMAN)
            continue;
        winner = ids[i];
    }
    force_mood(3);
    popup_country(0, 1, loser);
    set_name_slot_1038_8775(1, g_nation_text[loser].country);
    popup_say(2, get_name(winner));
    popup_say(3, get_name(loser));
    popm("SUCCESSION", 2);
    p3 = site_loc(0, 0);
    p2 = continent_loc(0, 0);
    lbit = 0x10 << loser;
    wbit = 0x10 << winner;
    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++) {
            if (*p3 & lbit)
                *p3 |= (unsigned char)wbit;
            if (((*p2 & 0xf0) >> 4) == loser) {
                *p2 &= 0xf;
                *p2 |= (unsigned char)winner << 4;
            }
            p3++;
            p2++;
        }
    }
    for (u = g_game.unit_count - 1; u >= 0; u--) {
        if (NATION(g_units[u]) != (unsigned char)loser)
            continue;
        x = g_units[u].x;
        y = g_units[u].y;
        if (!on_map(x, y)) {
            delete_unit(u);
            continue;
        }
        g_units[u].flags = (unsigned char)winner;
        owner_set(x, y, winner);
    }
    for (i = g_game.colony_count - 1; i >= 0; i--) {
        set_colony(i);
        if (g_cur_colony_rec->nation != (unsigned char)loser)
            continue;
        g_cur_colony_rec->nation = winner;
        g_cur_colony_rec->bells = 0L;
        owner_set(g_cur_colony_rec->x, g_cur_colony_rec->y, winner);
    }
    for (i = 0; g_game.settlement_count > i; i++) {
        set_village(i);
        if (((int)g_cur_settlement_rec->mission & MISSION_NATION_MASK) != loser)
            continue;
        g_cur_settlement_rec->mission &= 0xf0;
        g_cur_settlement_rec->mission |= (unsigned char)winner;
    }
    collect_stats(loser);
    collect_stats(winner);
    g_nation_text[loser].control = CONTROL_DEFEATED;
    g_game.mother_country_idx = loser;
    DrawMap(1);
}
