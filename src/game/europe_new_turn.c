/* PORT: this file's own declarations of fill_docks removed: the header declares it from the definition (W1 step 3). */
/* europe_new_turn: candidate for 1048:7f7c (581 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One nation's Europe step for the turn: market prices, immigration, and the King's war
 * declaration. Nothing happens at all once bit 0 of SEG20:0x7784 is set -- the same
 * independence latch add_liberty (§116) and negotiate_ai (§115) are gated on.
 *
 * Immigration is the bulk of it. `europe_religious_threshold` returns the crosses NEEDED and, by
 * reference, the crosses GAINED; the gained are added to +0x2e, floored at zero, and when
 * they pass the need an emigrant arrives. Which one is one of the three waiting at +2, +3
 * and +4, chosen by `rand_range(0, 2)`, and the slot it vacates is refilled immediately --
 * so the dock always shows three.
 *
 * The message is `UNREST`, and `TUTORIAL5` follows it once, gated on bit 7 of 0x7784 and
 * bit 0 of 0x7789 together, after which bit 8 of 0x7788 latches it off.
 *
 * NAME: descriptive. The Macintosh `europe_recruit(short, short)` takes two arguments and
 * `tools/signatures.py` reads TWO argument bytes -- one int -- off this function's call
 * site, so the arity excludes it. `europe_immigration` would name the largest branch rather
 * than the function, which also opens the market and can end in a declaration of war.
 *
 * Six spellings the bytes fixed, and five of them are one-byte tells.
 *
 * The function ends with an explicit `return;`, which is what produces the DEAD second
 * epilogue at the end -- nine bytes the census counts and Ghidra's reachable-code walk does
 * not. Without it the reconstruction stops at 572 of 581 and reads as complete.
 *
 * `g_cur_nation_rec` and `g_cur_nation` are NEAR in this module: `LES BX,[0x4cc4]` with no
 * segment set up, where the `* ` five other files declare costs four bytes of
 * segment immediate first. The same per-module split as §123's find_city, in the other
 * direction.
 *
 * The three recruit slots are `unsigned char`: the shipped promotion is `MOV AH,0`, where
 * `char` gives `CBW`.
 *
 * The two `popup_say` calls are indexed by DIFFERENT things -- the first by the NATION
 * (`MOV BX,SI`, stride 2 at SEG20:0xd8ae), the second by the recruit's SPECIALTY (a job id; JOB_COLONIST_ALT is shown as JOB_COLONIST) (`MOV BX,DI`,
 * `SHL BX,3` into the 8-byte records at 0xd61a). They sit four instructions apart and read
 * as a pair; only the register says they are not.
 *
 * And the tutorial gate is `(x & 1) == 0` and not `!(x & 1)`: the shipped code tests memory
 * directly, where the `!` form loads the byte and promotes it first (§114, seen here from
 * the side that wanted the memory-direct spelling).
 *
 * RENAMED 2026-09-26 from europe_nation_turn: the Mac build's europe_new_turn(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller new_turn_bookkeeping; callee europe_new_taxes; callee europe_recruit; callee force_mood; callee has_father), the runner-up gets 6, and the arity agrees (1).
 */

#include "colonization.h"

#include "w16types.h"
#include "fathers.h"
#include "control.h"
#include "nations.h"
#include "tutflag.h"
#include "gameflag.h"
#include "jobs.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern GameState g_game;   /* SEG20:0x7782 */

extern void random_activate(int seed);
extern void set_mood(int mood);
extern void force_mood(int mood);
extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int unit_from_docks(int type);
extern void move_to_bottom(int u);
extern void popup_say(int slot, int v);
extern int popm(char *msg, int n);

extern void set_europe(int n);
extern void europe_adjust_prices(int reset, int only);
extern int europe_religious_threshold(int nation, int *gained);

extern int europe_new_taxes(void);
extern void europe_new_wars(void);

void europe_new_turn(int nation)
{
    int need;
    int unit;
    int gained;
    int slot;
    register int n;
    int type;

    n = nation;
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        goto out;
    set_europe(n);
    random_activate(g_random_seed);
    g_cur_nation_rec->flags &= ~0x20;
    europe_adjust_prices(0, -1);
    need = europe_religious_threshold(g_cur_nation, &gained);
    g_cur_nation_rec->need = need;
    g_cur_nation_rec->crosses += gained;
    g_cur_nation_rec->crosses = max_int_1048_dd61(0, g_cur_nation_rec->crosses);
    if (g_cur_nation_rec->crosses > g_cur_nation_rec->need) {
        if (n < NATION_EUROPEAN_COUNT && g_nation_text[n].control == CONTROL_HUMAN) {
            set_mood(2);
            if (g_sound_music == 0)
                force_mood(2);
        }
        if (has_father(n, FATHER_WILLIAM_BREWSTER)) {
            europe_recruit(0, 1);
        } else {
            g_cur_nation_rec->crosses = 0;
            slot = rand_range(0, 2);
            type = g_cur_nation_rec->recruit[slot];
            g_cur_nation_rec->recruit[slot] = fill_docks(!(g_game.turn_counter & 3));
            unit = unit_from_docks(type);
            if (unit >= 0) {
                move_to_bottom(unit);
                if (n < NATION_EUROPEAN_COUNT && g_nation_text[n].control == CONTROL_HUMAN) {
                    if (type == JOB_COLONIST_ALT)
                        type = JOB_COLONIST;
                    popup_say(0, g_nation_name_word[n]);
                    popup_say(1, g_spec_recs[type].expert);
                    popm("UNREST", 4);
                    if ((g_game.game_flags & GAMEFLAG_TUTORIAL) && (HI_BYTE(g_game.hint_flags) & TUT_HI_SHOWN_5) == 0) {
                        popm("TUTORIAL5", 0);
                        g_game.hint_flags |= TUT_SHOWN_5;
                    }
                }
                g_cur_nation_rec->flags |= 0x40;
            }
        }
        if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN)
            g_show_europe = 1;
    }
    if (europe_new_taxes() == 0)
        europe_new_wars();
out:
    return;
}
