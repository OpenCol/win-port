/* PORT: this file's own declarations of is_enemy removed: the header declares it from the definition (W1 step 3). */
/* negotiate_ai: candidate for 1030:50d4 (336 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: negotiate_ai
 *
 * The periodic diplomacy step between two European powers: it either signs a treaty or
 * breaks one, and says so with one of three text resources -- `SIGNTREATY` (DGROUP:0x1509),
 * `CANCELPEACE` (0x1514) and `DECLAREWAR` (0x1520), all read out of the shipped image.
 *
 * Nothing happens at all once bit 0 of SEG20:0x7784 is set, which is the same word
 * `intervention` latches bit 1 of -- so both are gated on the same
 * end-of-game state.
 *
 * Otherwise: a two-in-three gate, `(a + b + g_game.turn_counter) % 3`, lets a pair with flag 0x20
 * through only on the third of the values it can take -- the TURN COUNTER (SEG20:0x7790) is
 * the varying term,
 * so the same pair gets a different answer as the game goes on. Flag 2 on either side stops
 * it outright. Then the two names are pushed into text slots 0 and 1, and the decision is
 * `is_enemy` in BOTH directions:
 *
 *   neither side returns non-zero    and neither already has 0x40  ->  SIGNTREATY, 0x40 set both ways
 *   either side returns non-zero     and (0x40 set, or 0x20 clear) ->  CANCELPEACE if 0x40 was set,
 *                                                                       else DECLAREWAR; 0x40 cleared
 *
 * So 0x40 is the treaty bit and the message depends on whether there was one to break.
 *
 * THE MACINTOSH BUILD CORROBORATES THE THREE HELPERS BY ARITY. It carries a `treaty_*`
 * family -- `get_treaty(short, short)`, `treaty_on(short, short, short)`,
 * `treaty_off(short, short, short)` -- and this function's three helpers are
 * `get_treaty(a, b)`, `treaty_on(a, b, bits)` and
 * `treaty_off(a, b, bits)`: two arguments, three, three, in that order.
 * Those names were derived descriptively from the bytes in an earlier session and the
 * Macintosh set agrees with all three independently. They are NOT renamed here --
 * `get_treaty` says what the bytes do and `get_treaty` says what it is for, and
 * the corroboration is worth more recorded than spent.
 *
 * Three divergences, all rules: the three early exits are `goto` and not `return` (rule 12,
 * a `JMP` to the single epilogue); `get_treaty` is TLINK's 5-byte form and stays
 * `extern`; and the last flag test is `|| !(x & 0x20)`, not `|| (x & 0x20)` -- the shipped
 * `JNZ` past the test is the tell, and the negation is easy to lose in a chain of four
 * tests on one value.
 *
 * RENAMED 2026-09-26 from nation_treaty_step: the Mac build's negotiate_ai(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller negotiate_europe; callee popm; callee popup_say), the runner-up gets 2, the arity agrees (2), and the Mac body is 1.21x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says.
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern void popup_say(int slot, int v);
extern int popm(char *msg, int n);
extern int treaty_on(int nation, int other, int bits);
extern int treaty_off(int nation, int other, int mask);

extern int get_treaty(int nation_a, int nation_b);
extern void unfortify(int nation, int owner);

void negotiate_ai(int nation, int other)
{
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        goto done;
    if ((nation + other + g_game.turn_counter) % 3 != 0 && (get_treaty(nation, other) & 0x20))
        goto done;
    if ((get_treaty(nation, other) & 2) || (get_treaty(other, nation) & 2))
        goto done;
    popup_say(0, get_adjective(nation));
    popup_say(1, get_adjective(other));
    if (is_enemy(nation, other) == 0 && is_enemy(other, nation) == 0) {
        if (!(get_treaty(nation, other) & 0x40) && !(get_treaty(other, nation) & 0x40)) {
            popm("SIGNTREATY", 2);
            treaty_on(nation, other, 0x40);
            unfortify(nation, other);
            unfortify(other, nation);
        }
    } else {
        if ((get_treaty(nation, other) & 0x40) || !(get_treaty(nation, other) & 0x20)) {
            if (get_treaty(nation, other) & 0x40)
                popm("CANCELPEACE", 2);
            else
                popm("DECLAREWAR", 2);
            treaty_off(nation, other, 0x40);
        }
    }
done:
    ;
}
