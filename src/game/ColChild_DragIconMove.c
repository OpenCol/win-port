/* ColChild_DragIconMove: candidate for 1048:179d (388 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DragIconMove
 *
 * Moves the dragged icon to (x, y) inside the owner's rectangle (+0x76a, InRect):
 * the saved background (port at DGROUP:0x4cbc) is put back where the icon was (the two
 * rectangles in data segment 30), the icon rectangle recentred on the pointer, the new
 * background saved, the icon sprite (DGROUP:0x4cb8) drawn through Sprite_Draw, and both
 * the new and the old areas flushed to the screen.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







extern void *GetEventWindow(void);
extern GRPort *g_drag_port;        /* DGROUP:0x4cbc */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int InRect(RECT *r, int x, int y);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern "C" void copy_rect_to_screen(void *win, RECT *r);
void ColChild_DragIconMove(int x, int y)
{
    ColChild *o;
    RECT r3;
    RECT r2;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    if (InRect(&o->rect, x, y)) {
        GRPort_CopyToPort(g_drag_port, (GRPort *)o, &g_drag_rect_src, g_drag_rect_dst);
        CopyRect(&r2, g_drag_rect_dst);
        SetRect(g_drag_rect_dst, x - (rect_width_1048_d69b(&g_drag_rect_src) >> 1), y - (rect_height_1048_d6ba(&g_drag_rect_src) >> 1),
                x - (rect_width_1048_d69b(&g_drag_rect_src) >> 1) + rect_width_1048_d69b(&g_drag_rect_src),
                y - (rect_height_1048_d6ba(&g_drag_rect_src) >> 1) + rect_height_1048_d6ba(&g_drag_rect_src));
        GRPort_CopyToPort((GRPort *)o, g_drag_port, g_drag_rect_dst, &g_drag_rect_src);
        Sprite_Draw(&r3, g_drag_sprite, (GRPort *)o, g_drag_rect_dst[0].left, g_drag_rect_dst[0].top);
        copy_rect_to_screen(o, &r3);
        copy_rect_to_screen(o, &r2);
    }
}
