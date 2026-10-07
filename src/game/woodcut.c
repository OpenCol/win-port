/* woodcut: candidate for 1060:0508 (223 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Show woodcut `n` once. A positive `n` that has already been shown returns 1 immediately;
 * otherwise it is marked shown, the per-woodcut side effect runs, and `woodcut_show` draws it.
 *
 * The side effects are a dense 0..10 switch, so Borland emits a jump table and puts it in the
 * CODE segment past the last instruction. Four of the eleven cases (3..6) are `force_music`
 * with a different unit type -- 0x33, 0x35, 0x36, 0x39 -- case 2 is an arrival (`new_mood`),
 * cases 0, 1 and 9 are a mood, and 7, 8 and 10 do nothing at all. The empty cases are in the
 * shipped table as their own entries (each a `jmp short` to the join), which is what keeps the
 * table dense and the dispatch a table rather than a compare chain.
 *
 * Cases 0 and 1 share the only conditional arm: the shipped table at 1060:05d2 is
 * 545,545,56b,577,583,58f,59b,5a7,5a9,5ab,5b7 -- entries 0 and 1 both name 0x545 (the grader
 * masks jump-table entries, so only tools/check_data_refs.py sees the pairing). The mood plays
 * only when DGROUP:0xb8 is set and 0xba is clear, the same pair `force_music` tests before it
 * queues a refresh.
 *
 * `n <= 0` skips the seen-bit entirely and falls straight into the switch, so a non-positive
 * woodcut can be shown repeatedly.
 *
 * SIZE: 223, not the census 195 and not the 224-byte gap. 196 bytes of code, a six-byte dead
 * second epilogue from the written-out `return`, and the eleven two-byte jump-table entries
 * the shipped `JMP [CS:BX+0x5d2]` indexes come to 223; the 224th byte is padding before the
 * next function and belongs to neither.
 *
 * The three module helpers are -called and are stubs here (rule 47); the three
 * segment-1040 routines are calls and are `extern`.
 *
 * RENAMED 2026-09-27 from woodcut_show: Call graph: Mac woodcut(short)'s sequence is woodcut_get, woodcut_set, force_mood, new_mood, force_music x4, force_mood, woodcut_show, and 0508's body calls woodcut_get, woodcut_set, force_mood x2, new_mood, force_music x4 and then 003a (becoming woodcut_show). Callers: Mac woodcut is called by combat_fight, explore_around_square, lost_city_exploration, negotiate_europe, negotiate_tribe, perform_orders_build, process_off_board_ships and tribe_village. 0508 has exactly those eight callers plus land_ho. Arity: (short) against woodcut_show(int hint). Size is 230 Mac against 223 Windows. Layout order agrees: it is last in the family on both sides. It is the other half of the 003a swap.
 */

#include "colonization.h"

extern void force_mood(int mood);
extern void new_mood(int mood);
extern void force_music(int type);

/* same object: 4-byte `push cs; call ` */

int woodcut(int hint)
{
    register int i;
    register int r;

    i = hint;
    r = 1;
    if (i > 0) {
        if (woodcut_get(i) != 0)
            goto out;
        woodcut_set(i, 1);
    }
    switch (i) {
    case 0:
    case 1:
        if (g_sound_event_music != 0 && g_sound_music == 0)
            force_mood(2);
        break;
    case 2: new_mood(2); break;
    case 3: force_music(0x33); break;
    case 4: force_music(0x35); break;
    case 5: force_music(0x36); break;
    case 6: force_music(0x39); break;
    case 7: break;
    case 8: break;
    case 9: force_mood(2); break;
    case 10: break;
    }
    r = woodcut_show(i);
out:
    return r;
}
