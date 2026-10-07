/* CTable_CopyRawColors: candidate for 1068:0652 (40 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_CopyRawColors
 * name-source: sibling-build
 *
 * A forwarder in the graphics library for MSCopyRawColors (1070:04ea): six words
 * pushed straight through, same signature.
 *
 * RENAMED 2026-09-26 from pack_palette_rgb_fwd: the Mac build's CTable::CopyRawColors(unsigned char*, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DoEndofTurn; caller Init; caller exec; caller begin_game), the runner-up gets 2, and the arity agrees (4).
 */

#include "colonization.h"

#include "w16types.h"
struct logpal;
#include "classes.h"
extern void MSCopyRawColors(struct logpal *pal, unsigned char *dst, int first, int count);

void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count)
{
    MSCopyRawColors((struct logpal *)pal, dst, first, count);
}
