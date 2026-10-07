/* ScaleMult: candidate for 1068:99e7 (214 bytes, 12 call sites).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks a cumulative table -- an ascending `int *` held in DGROUP at 0x5694 --
 * forward from index `start` until the running value reaches `table[start] + |amount|`, and
 * returns how many steps that took, negated when `amount` was negative. When `exact` is zero
 * and g_scale_num < g_scale_den (DGROUP:0x2b0e, 0x2b10) the final step is given back if it
 * overshot; the other arm never gives it back.
 *
 * Three things the bytes settle that the decompiled C does not:
 *
 *   - `if (a == 0) return 0;` is a real return, not rule 12's goto: the epilogue is
 *     duplicated in full at 1068:9a00 rather than jumped to. That contrast is the
 *     cleanest example in the corpus of how the two are told apart.
 *   - `NEG AX`, so this file negates with `-a`. reconstructed/on_colony_map.c
 *     (1038:b52a) has `NOT AX / INC AX` for the same operation and needs `~x + 1`.
 *     Both idioms are present in this program, in different translation units, so
 *     rule 11 is about reading which one the bytes show and not about a house style.
 *   - **The two arms of the outer branch are -duplicates in the source.** Both
 *     assign the start index, both compute the limit and both run the same search
 *     loop; only the overshoot correction distinguishes them. The compiler does not
 *     merge them, and factoring them together in the source loses the duplicate
 *     `MOV CX,DI` at 1068:9a2a and 1068:9a6e. The redundancy is the programmer's.
 *
 * Verdict RELOCATABLE. Matched first attempt.
 * Re-verified 2026-09-10 at the full 214-byte extent, trailing dead epilogue included: RELOCATABLE.
 *
 * RENAMED 2026-09-27 from step_table_by: NOT PROPOSED BY mac_bind. Mac ScaleMult(short, short, short). BODY IDENTICAL: the Mac body is the same program as step_table_by. It returns 0 for amount 0, takes the sign, branches on scale_num < scale_den, walks the scale map from start until map[i] >= map[start]+amount, steps back one when not exact (small-scale arm only), and returns +/-(i - start). Callers: DrawSprite and DrawMonoSprite on Mac; sprite_draw_scaled and sprite_blit_mode here (chain 1), 6 calls each on both. Arity 3. Layout contrary but explained: Windows emits it among 1068's module-local helpers (rect_width_1068_99a9, sprite_width_1068_9abd), not between the two blits.
 */

#include "colonization.h"

int ScaleMult(int amount, int start, int exact)
{
    int limit;
    int neg;
    int i;

    if (amount == 0) return 0;
    if (amount < 0) {
        neg = 1;
        amount = -amount;
    } else
        neg = 0;
    if (g_scale_num < g_scale_den) {
        i = start;
        limit = g_scale_map[start] + amount;
        while (g_scale_map[i] < limit)
            i++;
        if (exact == 0 && g_scale_map[i] != limit)
            i--;
    } else {
        i = start;
        limit = g_scale_map[start] + amount;
        while (g_scale_map[i] < limit)
            i++;
    }
    return (neg ? -1 : 1) * (i - start);
}
