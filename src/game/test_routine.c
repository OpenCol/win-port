/* test_routine: candidate for 1048:b95a (74 bytes).  Verdict: RELOCATABLE.
 *
 * A Cheat-menu entry: clears bits 0, 1 and 3 of the flags word at SEG20:0x7784, puts the
 * unit count (SEG20:0x779e) and the colony count (SEG20:0x77a0) into dialog option slots
 * 0 and 1, and shows the `TEST` text resource.
 *
 * `TEST` is derived, not guessed. The literal is at DGROUP:0x2417; `GAMEOPTIONS` runs from
 * 0x240b and `MEMORY` starts at 0x241c, so this one is FOUR characters plus a NUL, and
 * `TEST` is the only four-character TEXT resource in the shipped table that fits. The
 * byte-match cannot confirm a literal -- the push is a fixup and masked -- so the length
 * arithmetic is the whole evidence, the same argument the twelve `append_*` siblings use.
 *
 * The three bits are the ones this branch has already named at that word: bit 0 is
 * independence declared and bit 1 the intervention already spent (add_liberty, §116;
 * intervention, §114). Clearing them is what a developer's reset button does.
 *
 * The mask is `& ~0xb` and not `& 0xfff4`: the shipped immediate is the sign-extended byte
 * 0xf4, which both spellings give, but ~0xb is the one that says which bits are meant.
 *
 * `set_text_arg_i_1048_db6a` and `ask_text_resource` are -called and live in this object.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern void set_text_arg_i_1048_db6a(int slot, int v);

void test_routine(void)
{
    g_game.game_flags &= ~0xb;
    set_text_arg_i_1048_db6a(0, g_game.unit_count);
    set_text_arg_i_1048_db6a(1, g_game.colony_count);
    ask_text_resource("TEST");
}
