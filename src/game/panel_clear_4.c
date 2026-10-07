/* panel_clear_4: candidate for 1008:357e (23 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Zeroes the word at +4 of the object it is handed.
 *
 * ITS SECOND ARGUMENT `n` IS NEVER READ. The caller passes `(&g_game_brains, u)`, so the second
 * parameter is the caller's claim and not this function's -- the body touches only the first
 * (RE_STATUS 119). The name says the offset because nothing establishes what the word is.
 */

#include "colonization.h"

#include "records.h"

void panel_clear_4(GameBrains *p, int n)
{
    p->phase = 0;
}
