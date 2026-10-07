/* nation_flag_is_clear: candidate for 1020:9857 (45 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The negation of `has_father`: 1 when the nation's flag bit is clear. Nothing calls
 * it -- it is declared global and reached only through the segment's export table.
 *
 * size-source: 45, not the census 41. The two `return`s give two epilogues and the last
 * 4 bytes are the unreachable one; Ghidra sizes by reachable code.
 */

#include "colonization.h"

extern int has_father(int nation, int bit);

int nation_flag_is_clear(int nation, int bit)
{
    if (has_father(nation, bit) != 0)
        return 0;
    return 1;
}
