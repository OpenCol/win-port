/* enemy_is_active: candidate for 1038:38ec (170 bytes, 4 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * True when unit n is a valid, on-map unit of the current nation that is either not
 * flagged 0x80 at +4 or is type 11, and whose rating exceeds its byte +5. The two
 * validity exits are `goto`s, which -Od renders as a conditional jump over a `JMP`.
 *
 * RENAMED 2026-09-26 from unit_outranks_f5: the Mac build's enemy_is_active(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between unit_is_active and unit_special_active, which are already bound; the arity agrees and the Mac body is 1.01x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern unsigned char move_rate(int u);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

int enemy_is_active(int unit)
{
    int u = unit;
    int r = 0;

    if (u < 0 || g_game.unit_count <= u)
        goto done;
    if ((int)(signed char)g_units[u].x < 0)
        goto done;
    r = NATION(g_units[u]) == ((unsigned char)g_game.turn_nation)
        && ((g_units[u].flags2 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
        && move_rate(u) > g_units[u].moves_used;
done:
    return r;
}
