/* ColChild_ExtractSprite: candidate for 1048:075d (34 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_ExtractSprite
 * name-source: sibling-build
 *
 * Stores the literal tile size 62 x 47 into a window's `tile_w` (+0x144) and `tile_h`
 * (+0x146) -- the names tools/layouts.json already holds for those two offsets in both the
 * `Win` and `Obj` views. The numbers are the whole of what this function knows; which window
 * lays its tiles out at 62 x 47 is its caller's business, so the name states the size.
 *
 * RENAMED 2026-09-26 from set_tile_size_62x47: the Mac build's ColChild::ExtractSprite(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between ColChild_ResizeCWin and ColChild_DrawNonClient, which are already bound; the arity agrees.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ColChild_ExtractSprite(ColChild *w)
{
    w->tile_w = 0x3e;
    w->tile_h = 0x2f;
}
