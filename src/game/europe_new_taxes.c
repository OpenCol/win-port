/* europe_new_taxes: candidate for 1048:7c13 (873 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The King's demand: five events chosen by one score, each with its own text key and its own
 * effect on the tax negotiation that follows. Below 100 it is "KINGVICTORY" and the tax goes
 * DOWN by 2..5 points (the amount is negated); below 0x28a and with fewer than 30 of whatever
 * SEG20:0x77a9 counts, "KINGWIFE" and that counter rises by one; below 0x3b6, "KINGWAR" with a
 * new country at SEG20:0x77aa, re-rolled until it differs from the old one; below 0x44c,
 * "KINGNAVACT" for 3 or 4 points; otherwise "KINGSTAMPACT" for 5 to 8.
 *
 * FOUR GATES BEFORE ANY OF IT. The player must have a colony, the TURN COUNTER (SEG20:0x7790,
 * which earlier headers called the treasury) must be at least 30, and it must be an exact
 * multiple of a period that SHRINKS with the year -- 0x12, less 3
 * at each of 1600, 1700 and 1750, less twice the difficulty above 2 for a human-controlled European power -- and
 * the current tax rate must be 0x55 or under. So demands get more frequent as the game goes on
 * and stop entirely at an 85 percent tax.
 *
 * The score is a 1..1000 roll less five times the tax rate, plus the nation's byte at
 * DGROUP:0x4b54, plus a hundredth of the nation record's long at +0x2a, plus a thirtieth of the
 * turn counter, plus ten times
 * SEG20:0x77d2 -- so a rich, lightly taxed nation gets the expensive demands.
 *
 * TWO COMMA OPERATORS IN ONE CALL, and they are what closed the function: the `max` of the old
 * counter plus one and 1 is written `*max_ptr(&(b = 1, &b), &(a = x + 1, &a))`, with both stores
 * INSIDE the argument list. As separate statements Borland floats `PUSH SS` ahead of each `LEA`
 * and two bytes move. Rule 61.
 *
 * The 0x34-byte nation record is at SEG20:0x7810 here, where four other reconstructions index
 * the same array from 0x7841 -- that address is this record's +0x31.
 *
 * RENAMED 2026-09-26 from king_demand: the Mac build's europe_new_taxes(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee popup_say; callee raise_taxes; callee text_search), the runner-up gets 1, and the arity agrees (0).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void popup_say(int slot, int word);

extern void text_search(void *rec, char *key, int n);

/* same object: 4-byte `push cs; call ` */
extern void set_name_slot_1048_de36(int slot, void *s);

extern void raise_taxes(char *key, int delta);

int europe_new_taxes(void)
{
    int r;
    int div;
    int war_country;
    int period;
    int a;
    int b;
    char buf[0x50];
    register int score;
    register int n;

    r = 0;
    if (g_nation_colony_count[g_cur_nation] == 0)
        goto out;
    if (g_game.turn_counter < 0x1e)
        goto out;
    period = 0x12;
    if (g_game.year > 0x640)
        period -= 3;
    if (g_game.year > 0x6a4)
        period -= 3;
    if (g_game.year > 0x6d6)
        period -= 3;
    div = period - (g_cur_nation < 4 && g_nation_text[g_cur_nation].control == 0
                    ? g_game.difficulty - 2 : 0) * 2;
    if (g_game.turn_counter % div != 0)
        goto out;
    if ((int)g_cur_nation_rec->tax > 0x55)
        goto out;
    score = rand_range(1, 0x3e8);
    score -= (int)g_cur_nation_rec->tax * 5;
    score += g_nation_population[g_cur_nation];
    score += (int)(g_cur_nation_rec->gold / 100);
    score += g_game.turn_counter / 0x1e;
    score += g_game.rebel_sentiment * 10;
    popup_say(0, g_difficulty_words[g_game.difficulty]);
    set_name_slot_1048_de36(1, g_nation_text[g_cur_nation].leader);
    if (score < 0x64) {
        n = rand_range(2, 5);
        n = min_int_1048_db44(n, (int)g_cur_nation_rec->tax);
        if (n <= 0)
            goto out;
        n = -n;
        STRCPY(buf, "KINGVICTORY");
        text_search(g_game_txt, "COUNTRIES", (int)g_game.year_lo - 1);
        set_name_slot_1048_de36(2, g_text_buf);
    } else if (score < 0x28a && (int)g_game.year_hi < 0x1e) {
        n = 1;
        STRCPY(buf, "KINGWIFE");
        g_game.year_hi = *max_ptr_1048_efef((b = 1, &b), (a = (int)g_game.year_hi + 1, &a));
        text_search(g_game_txt, "ORDINAL", (int)g_game.year_hi - 1);
        set_name_slot_1048_de36(2, g_text_buf);
    } else if (score < 0x3b6) {
        n = 2;
        STRCPY(buf, "KINGWAR");
        do {
            war_country = rand_range(1, 8);
        } while ((int)g_game.year_lo == war_country);
        g_game.year_lo = (char)war_country;
        text_search(g_game_txt, "COUNTRIES", war_country - 1);
        set_name_slot_1048_de36(2, g_text_buf);
    } else if (score < 0x44c) {
        n = rand_range(3, 4);
        STRCPY(buf, "KINGNAVACT");
    } else {
        n = rand_range(5, 8);
        set_name_slot_1048_de36(2, g_nation_text[g_cur_nation].country);
        STRCPY(buf, "KINGSTAMPACT");
    }
    g_cur_nation_rec->last_tax_delta = n;
    raise_taxes(buf, n);
    r = 1;
out:
    return r;
}
