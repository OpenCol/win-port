/* GRPort_GetClip: candidate for 1068:4e88 (38 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_GetClip
 * name-source: sibling-build
 *
 * Copies the rectangle at +8 of the record out through F_SCOPY.
 *
 * RENAMED 2026-09-26 from get_rect_at_8: copies out the port's clip rectangle at +8 (GRPort_InitStruct's clip);
 * ColChild_DrawTitleBar and ColChild_DrawNonClient save it here before GRPort_SetClip narrows it.
 *
 * RENAMED 2026-09-26 from port_get_clip: the Mac build's GRPort::GetClip(Rect*): this plus a rectangle, filled with the clip at +8. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void GRPort_GetClip(GRPort *o, RECT *r)
{
    *r = o->rect;
}
