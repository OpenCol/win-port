/* PORT: this file's own declarations of copy_rect_to_screen removed: the header declares it from the definition (W1 step 3). */
/* ColChild_DragIconMUp: candidate for 1048:1921 (136 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DragIconMUp
 *
 * Mouse-up of an icon drag: the saved background (port at DGROUP:0x4cbc) is copied back
 * into the owning window between the two rectangles in data segment 30, that area is
 * flushed to the screen, the drop position is recorded at 0x4cc0/0x4cc2 and the window's
 * flag +0x760 set.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern GRPort *g_drag_port;      /* DGROUP:0x4cbc */

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
/* same object: 4-byte `push cs; call ` (rule 47) */
void ColChild_DragIconMUp(int x, int y)
{
    ColChild *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    GRPort_CopyToPort(g_drag_port, (GRPort *)o, &g_drag_rect_src, g_drag_rect_dst);
    copy_rect_to_screen(o, g_drag_rect_dst);
    g_drag_x = x;
    g_drag_y = y;
    o->dropped = 1;
}
