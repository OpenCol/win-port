/* king_adjust_tax: candidate for 1048:6a74 (251 bytes).  Verdict: RELOCATABLE.
 *
 * The King's tax move. With the market's tax byte (+1 of the record at DGROUP:0x4cc4) above 1
 * and above a limit of (turn / 400 + 1) * ((difficulty >> 1) + 1) * 4 plus 5: when it is
 * also above the limit itself and a 1-in-(difficulty + 1) roll hits, KINGLOWER by 1..(5 -
 * difficulty); otherwise KINGNOTHING. Below the limit: KINGRAISE by twice a roll of
 * 1..difficulty. (The TURN COUNTER is at SEG20:0x7790 -- `GameBrains_DoEndofTurn` increments it once
 * a turn -- and the difficulty byte at 0x77a8. Earlier headers called 0x7790 the treasury.)
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void raise_taxes(char *name, int delta);
void king_adjust_tax(void)
{
    int q;
    int step;
    int lim;
    int r;

    if ((int)g_cur_nation_rec->tax <= 1)
        goto raise;
    q = g_game.turn_counter / 400;
    step = ((g_game.difficulty >> 1) + 1) << 2;
    lim = (q + 1) * step;
    if (g_cur_nation_rec->tax <= (signed char)(lim + 5))
        goto raise;
    if (g_cur_nation_rec->tax <= (signed char)lim || rand_range(1, g_game.difficulty + 1) != 1) {
        ask_text_resource("KINGNOTHING");
        goto done;
    }
    r = 0 - rand_range(1, 5 - g_game.difficulty);
    raise_taxes("KINGLOWER", r);
    goto done;
raise:
    r = rand_range(1, g_game.difficulty) * 2;
    raise_taxes("KINGRAISE", r);
done:
    return;
}
