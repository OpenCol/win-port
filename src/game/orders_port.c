/* PORT: this file's own declarations of ask_port removed: the header declares it from the definition (W1 step 3). */
/* orders_port: candidate for 1008:14bf (179 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Go To Port order: ask which port, and either sail for Europe or send the selected
 * unit to that colony.
 *
 * `ask_port(unit, -1, 1, 0)` is the "Sail to which port?" picker, called here
 * with a real unit and `mark_here` set, so the colony the unit is standing in is refused.
 * It returns 0x3e7 for the European port, a colony index otherwise, and -1 when dismissed.
 *
 * Europe is the branch with the rule: after independence is declared (bit 0 of
 * SEG20:0x7784) you cannot go back, and the refusal is the TEXT message `EUROPENOTLEAVE`
 * at DGROUP:0x252 -- read out of the data segment, not guessed.
 *
 * A colony sets three bytes of the unit record and nothing else: the orders byte at +8 becomes 3,
 * and +9/+10 take the colony's map position. `orders_route` writes 2 into that same
 * byte for its own order, so 3 is "travelling to a fixed destination" and the two bytes
 * after it are that destination.
 *
 * `show_text_dialog_1008_cdf3` is reached by the four-byte PUSH CS / CALL NEAR form and is a stub here
 * (rule 47); the other three are calls and are `extern`.
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_course_for_europe(int unit);
extern void set_colony(int colony);

/* same object: 4-byte `push cs; call ` */

void orders_port(void)
{
    register int colony;
    register int unit;

    unit = g_game.selected_unit;
    colony = ask_port(unit, -1, 1, 0);
    if (colony == 0x3e7) {
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
            show_text_dialog_1008_cdf3("EUROPENOTLEAVE");
        else
            set_course_for_europe(unit);
    } else if (colony >= 0) {
        set_colony(colony);
        g_units[unit].orders = 3;
        g_units[unit].dest_x = g_colonies[colony].x;
        g_units[unit].dest_y = g_colonies[colony].y;
    }
}
