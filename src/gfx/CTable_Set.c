/* CTable_Set: candidate for 1068:067a (59 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_Set
 * name-source: sibling-build
 *
 * Sets one palette entry and installs it at once: `MSSetCTEntry(pal, i, r, g, b)` then
 * `CTable_UpdatePal(pal, i, 1)`. The single-entry form of `CTable_ResetAnimate`.
 *
 * `i` is held in SI from entry because it is used twice (a `register` parameter). r, g and b
 * are `unsigned char`, as on the Mac and in MSSetCTEntry's definition. A char passed on to a
 * char parameter is still pushed as the whole word (`PUSH [BP+x]`); the old `int` spelling
 * came from declaring MSSetCTEntry with `int` here, and char against that callee widens with
 * `MOV AH,0` (substitution, 2026-09-28). MSSetCTEntry is a call; CTable_UpdatePal is the four-byte form
 * and is defined here (rule 47).
 *
 * RENAMED 2026-09-26 from palette_set_entry_1068_067a: the Mac build's CTable::Set(short, unsigned char, unsigned char, unsigned char): this plus an index and r, g, b, in that order. The platform module's MSSetCTEntry (1070:0342) is a different routine and keeps its name. It sits in the CTable module (1068:0000..0d94) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
struct logpal;
#include "classes.h"
extern void MSSetCTEntry(struct logpal *pal, int i, unsigned char r, unsigned char g, unsigned char b);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_UpdatePal(CTable *pal, int start, int count);

void CTable_Set(CTable *pal, register int i, unsigned char r, unsigned char g, unsigned char b)
{
    MSSetCTEntry((struct logpal *)pal, i, r, g, b);
    CTable_UpdatePal(pal, i, 1);
}
