/* PORT: this file's own declarations of KingRoom removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of popk, text_search removed: the header declares them from the definition (W1 step 3). */
/* end_of_game_check: candidate for 1008:1e2c (2811 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 2811, not the census 2809. The compiled function is exactly 2811 bytes and the shipped
 * ends on the `RETF` two bytes further on, which is also the gap to the next function.
 *
 * EVERY WAY THE GAME CAN END, checked once a year. `GameBrains_Init`'s turn loop calls it and it decides,
 * in this order: have you lost by 1600, have you won independence, how badly are you doing, has
 * any other nation been granted its own independence, and is it time to retire.
 *
 * 1600 WITH NO COLONIES ends it -- LOSENOCOLONIES -- unless the War of Independence is already on.
 * The colony count is recomputed from scratch first, because this is the only place that keeps
 * DGROUP:0x4b48 honest.
 *
 * WINNING INDEPENDENCE needs three things at once, and each of them can be waived by bit 5 of
 * SEG20:0x7784: the King must hold no colonies, his three land forces together must be under a
 * threshold (8 with bit 6 set, otherwise 1), and regulars plus one-for-any-cavalry plus
 * one-for-any-artillery must be under 4. Then WINNING, KINGLOSE, the quit flag and the game is over.
 *
 * OTHERWISE A REPORT, and the way it is built is the nicest thing in the function: the resource name
 * is patched by ARITHMETIC ON ITS LAST DIGIT. "WARN0" becomes WARN1, WARN2 or WARN3 by adding the
 * warning level to `msg[4]`, and "LOSING0" becomes LOSING1..3 by adding the defeat level to `msg[6]`.
 * Levels 1 and 2 are colony counts (fewer than 3, or none at all) and level 3 is the strength ratio:
 * the King's power over the sum of both, at 80 percent for a warning and 90 percent for defeat.
 *
 * THE OTHER NATIONS (not human-controlled, not already independent) are each granted their OWN independence
 * at (4 - difficulty) * 10 + 40 percent support, and the support number is that nation's own DGROUP:0x4b54 byte
 * scaled by its +0x19 and capped at 100. Twenty
 * points short of the threshold they say OTHERMIGHT, and five points of backsliding says OTHERLESS;
 * each message updates the +0x1a they remember, so neither repeats without a change. Independence
 * RENAMES THE NATION -- the new name comes out of NAMES/INDEPENDENT and is copied over its country
 * string -- sets bit 2 of its record, and makes peace with the other three at once.
 *
 * RETIREMENT is offered at 1790 (or 1840 once independent) with SOONRETIRING, and forced at 1800 (or
 * 1850) with RETIRING or RETIRING2, followed by the Hall of Fame. The largest colony you own is found
 * first so the text can name it.
 *
 * SCORED is the last word: only when something above has CLEARED SEG20:0x77c4 (and left `ask` set), the player is asked, and
 * answer 2 keeps the game alive while anything else sets the quit flag. The palette is captured at the
 * top and put back at the bottom whatever happens.
 *
 * SHIPPED ODDITIES. `total = 0;` is followed immediately by `total = g_game.royal_forces[0];`. The `n < 4`
 * test inside `for (n = 0; n < 4; n++)` can never fail. And the absolute value taken of the two power
 * bytes can never be negative, because they are unsigned chars widened with `MOV AH,0` -- the negating
 * arm of the macro is unreachable.
 *
 * WHAT THE BYTES SETTLED: the power bytes reach `ABS` through an explicit `(int)`, which is what turns
 * an unsigned byte compare into `OR AX,AX / JNG`. The best-colony size test is a BYTE compare against
 * `(char)bestsize` and a structured `if`, where the nation test one line above it is a `continue`. The
 * inner nation loop compares `n == k`, not `k == n`, and the operand order shows in `CMP SI,DI`. And
 * the retirement arm ends with an explicit `goto` to the join, which is rule 54's `EB 00`: two bytes
 * of nothing that no fall-through would produce.
 *
 * RENAMED 2026-09-26 from end_of_year_checks: the Mac build's end_of_game_check(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller DoEndofTurn; callee CopyRawColors; callee Fill; callee popup_say; callee say_number), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "royal.h"
#include "gameflag.h"
#include "classes.h"
#define ABS(d) ((d) > 0 ? (d) : ~(d) + 1)

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

struct logpal { char pad[1]; };
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);
extern void collect_stats(int nation);
extern void set_colony(int colony);
extern void set_europe(int n);
extern void popup_say(int slot, int word);
extern void popup_country(int slot, int flag, int nation);
extern int popm(char *msg, int n);
extern void FinalScore(void);
extern void force_mood(int mood);

extern int treaty_on(int nation, int other, int mask);
extern int treaty_off(int nation, int other, int mask);

/* 5-byte NOP form, so extern (rule 47) */
extern void OpenBackdrop(void);
extern void CloseBackdrop(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

void end_of_game_check(void)
{
    register int n;
    register int other;
    int i;
    int ports;
    int losing;
    int king_power;
    int player_power;
    int pct;
    int bestsize;
    int bestcol;
    int ans;
    int total;
    int warning;
    int want;
    int att;
    int ask;
    char msg[0x50];
    char pal[0x2c4];

    ask = 1;
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        collect_stats(g_game.mother_country_idx);
    collect_stats(g_game.player_nation);
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), pal, 10, 0xec);
    g_nation_colony_count[g_game.player_nation] = 0;
    for (i = 0; i < g_game.colony_count; i++) {
        if (g_colonies[i].nation == (unsigned char)g_game.player_nation)
            g_nation_colony_count[g_game.player_nation]++;
    }
    if (g_game.year >= 0x640 && g_nation_colony_count[g_game.player_nation] == 0
        && !(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
        popup_say(0, g_difficulty_words[g_game.difficulty]);
        set_name_slot_1008_cdca(1, g_nation_text[g_game.player_nation].leader);
        popk("LOSENOCOLONIES");
        OpenBackdrop();
        if (!(g_game.game_flags & GAMEFLAG_SCORED))
            FinalScore();
        g_game.game_in_progress = 0;
        g_quit_flag = 0;
        ask = 0;
        CloseBackdrop();
        MSDoAllTasks_1008_ce3f();
        goto done;
    }
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && !(g_game.game_flags & GAMEFLAG_REVOLUTION_WON)) {
        if (g_nation_colony_count[g_game.mother_country_idx] == 0 || (g_game.game_flags & GAMEFLAG_TUTORIAL17_SHOWN)) {
            total = 0;
            total = g_game.royal_forces[ROYAL_REGULARS];
            total += g_game.royal_forces[ROYAL_CAVALRY];
            total += g_game.royal_forces[ROYAL_ARTILLERY];
            if ((g_game.game_flags & GAMEFLAG_WAR_BEGUN ? 8 : 1) > total || (g_game.game_flags & GAMEFLAG_TUTORIAL17_SHOWN)) {
                if ((g_game.royal_forces[ROYAL_ARTILLERY] != 0) + g_game.royal_forces[ROYAL_REGULARS]
                        + (g_game.royal_forces[ROYAL_CAVALRY] != 0) < 4 || (g_game.game_flags & GAMEFLAG_TUTORIAL17_SHOWN)) {
                    set_name_slot_1008_cdca(0, g_nation_text[g_game.player_nation].leader);
                    set_name_slot_1008_cdca(1, g_nation_text[g_game.player_nation].country);
                    force_mood(3);
                    show_text_dialog_1008_cdf3("WINNING");
                    OpenBackdrop();
                    KingRoom(1, 2, "KINGLOSE");
                    g_game.game_flags |= 8;
                    g_game_over = 1;
                    g_quit_flag = 1;
                    MSDoAllTasks_1008_ce3f();
                    goto done;
                }
            }
        }
        ports = 0;
        for (i = 0; i < g_game.colony_count; i++) {
            set_colony(i);
            if (g_cur_colony_rec->nation != (unsigned char)g_game.player_nation)
                continue;
            if (g_cur_colony_rec->status & 0x40)
                ports++;
        }
        losing = 0;
        warning = 0;
        if (ports < 3)
            warning = 1;
        if (ports == 0)
            losing = 1;
        king_power = ABS((int)g_pop_by_nation[g_game.mother_country_idx]) + 1;
        player_power = ABS((int)g_pop_by_nation[g_game.player_nation]) + 1;
        pct = king_power * 100 / (king_power + player_power);
        if (pct >= 0x50)
            warning = 3;
        if (pct >= 0x5a)
            losing = 3;
        if (g_nation_colony_count[g_game.player_nation] < 3)
            warning = 2;
        if (g_nation_colony_count[g_game.player_nation] == 0)
            losing = 2;
        if (losing != 0) {
            set_name_slot_1008_cdca(0, g_nation_text[g_game.player_nation].country);
            set_name_slot_1008_cdca(1, g_nation_text[g_game.player_nation].leader);
            popup_country(2, 0, g_game.intervention_nation);
            STRCPY(msg, "LOSING0");
            msg[6] += (char)losing;
            show_text_dialog_1008_cdf3(msg);
            popup_say(0, g_nation_name[g_game.player_nation]);
            OpenBackdrop();
            KingRoom(2, 1, "KINGWIN");
            if (!(g_game.game_flags & GAMEFLAG_SCORED))
                FinalScore();
            g_game.game_in_progress = 0;
            g_quit_flag = 1;
            ask = 0;
            MSDoAllTasks_1008_ce3f();
            goto done;
        } else if (warning != 0) {
            STRCPY(msg, "WARN0");
            msg[4] += (char)warning;
            set_name_slot_1008_cdca(0, g_nation_text[g_game.player_nation].country);
            set_text_arg_i_1008_cda6(0, ports);
            set_text_arg_b_1008_ce52(1, g_nation_colony_count[g_game.player_nation]);
            set_text_arg_i_1008_cda6(2, pct);
            popm(msg, 1);
        }
    }
    if (!(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) {
        for (n = 0; n < 4; n++) {
            if (n < 4 && g_nation_text[n].control == 0)
                continue;
            set_europe(n);
            if (g_cur_nation_rec->flags & 4)
                continue;
            want = (4 - g_game.difficulty) * 10 + 40;
            att = g_nation_population[n] * g_cur_nation_rec->support / 100;
            att = min_int_1008_ce76(100, att);
            if (att >= want) {
                popup_country(0, 0, n);
                set_name_slot_1008_cdca(1, g_nation_text[n].country);
                set_name_slot_1008_cdca(2, g_nation_text[n].leader);
                text_search("NAMES", "INDEPENDENT", n);
                STRCPY(g_nation_text[n].country, g_text_buf);
                set_name_slot_1008_cdca(3, g_text_buf);
                show_text_dialog_1008_cdf3("OTHERGRANTED");
                g_cur_nation_rec->flags |= 4;
                for (other = 0; other < 4; other++) {
                    if (n == other)
                        continue;
                    treaty_on(n, other, 0x40);
                    treaty_off(n, other, 0xbb);
                }
                continue;
            }
            if (want - 20 <= att && (int)g_cur_nation_rec->shown < att) {
                popup_country(0, 0, n);
                set_text_arg_i_1008_cda6(0, att);
                set_text_arg_b_1008_ce52(1, g_nation_population[n]);
                set_text_arg_i_1008_cda6(2, want);
                popup_say(1, get_adjective(n));
                popm("OTHERMIGHT", 2);
                g_cur_nation_rec->shown = att;
            }
            if ((int)g_cur_nation_rec->shown - 5 > att) {
                popup_country(0, 0, n);
                set_text_arg_i_1008_cda6(0, att);
                set_text_arg_b_1008_ce52(1, g_nation_population[n]);
                set_text_arg_i_1008_cda6(2, want);
                popup_say(1, get_adjective(n));
                popm("OTHERLESS", 2);
                g_cur_nation_rec->shown = att;
            }
        }
    }
    if (!(g_game.game_flags & GAMEFLAG_SCORED)) {
        if (g_game.season == 0) {
            if ((g_game.year == 0x6fe && !(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) || g_game.year == 0x730) {
                popup_say(0, g_difficulty_words[g_game.difficulty]);
                set_name_slot_1008_cdca(1, g_nation_text[g_game.player_nation].leader);
                STRCPY(msg, "SOONRETIRING");
                if (g_game.year == 0x6fe)
                    say_number(msg, 0);
                else
                    say_number(msg, 1);
                popm(msg, 1);
            }
        }
        if ((g_game.year == 0x708 && !(g_game.game_flags & GAMEFLAG_INDEPENDENCE)) || g_game.year == 0x73a) {
            bestsize = 0;
            bestcol = 0;
            for (i = 0; i < g_game.colony_count; i++) {
                set_colony(i);
                if (g_cur_colony_rec->nation != (unsigned char)g_game.player_nation)
                    continue;
                if (g_cur_colony_rec->pop >= (signed char)bestsize) {
                    bestsize = g_cur_colony_rec->pop;
                    bestcol = i;
                }
            }
            popup_say(0, g_difficulty_words[g_game.difficulty]);
            set_name_slot_1008_cdca(1, g_nation_text[g_game.player_nation].leader);
            set_name_slot_1008_cdca(2, g_colonies[bestcol].name);
            if (!(g_game.game_flags & GAMEFLAG_INDEPENDENCE))
                show_text_dialog_1008_cdf3("RETIRING");
            else
                show_text_dialog_1008_cdf3("RETIRING2");
            OpenBackdrop();
            FinalScore();
            CloseBackdrop();
            g_game.game_in_progress = 0;
            g_quit_flag = 0;
            goto done;
        }
    }
extern done: if (g_game.game_in_progress == 0);
    CloseBackdrop();
    CTable_Fill(&g_game_palette, 10, 0xec, pal);
    MSDoAllTasks_1008_ce3f();
}
