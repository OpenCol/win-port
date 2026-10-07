/* revolution_setup: candidate for 1038:4f4a (35 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Gives the MOTHER COUNTRY its own map colour, 14, at the index `g_game.mother_country_idx`
 * holds -- which is what makes the power you rebelled against look different from the
 * other three. `anti_revolution` fills the same array's first four entries with
 * 10..13, and `revolution` calls this, as does `load_game_file` for a game saved after independence.
 *
 * RENAMED 2026-09-26 from set_mother_country_colour: the Mac build's revolution_setup(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between purge_square and get_strength, which are already bound, paired in order; the arity agrees and the Mac body is 1.43x this one.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

void revolution_setup(void)
{
    g_nation_colour[g_game.mother_country_idx] = 0xe;
}
