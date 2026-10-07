/* show_backdrop: candidate for 1050:6d32 (321 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_backdrop
 *
 * Puts half of picture resource 0xc9 on the window. The picture is 0x500 x 0x1e0 -- 1280 by
 * 480, two screens side by side -- and `which` picks the half: 1 is the left 640, 2 the right.
 * The destination is the full 640x480 either way.
 *
 * `which` is REMEMBERED at +0xc4 (`shown_half`) and the -1 there means the window is hidden:
 * `win_hide_1050_6e73` hides it, shrinks its port to 1x1 and stores the -1. Only a call that finds
 * -1 grows the port back to 640x480, locks the graphics and shows the window. Every later call
 * just blits, so switching halves is cheap and the first one is not.
 *
 * Anything but 1 or 2 leaves the source rectangle uninitialised -- there is no `else` and no
 * default -- so the blit reads whatever was on the stack. It has no callers in the call
 * graph and no relocation targets it: with win_hide_1050_6e73 and hide_owner_window it is
 * dead code in this build, and the Mac build has no counterpart.
 *
 * The 0x41e-byte CTable and the 0x26-byte GRPort are both locals, which is what the 1138-byte
 * frame and the exception block are for. Borland lays the two objects out FIRST, then its own
 * 30-byte exception frame, then the two plain RECTs -- so the rectangles are at the bottom of
 * the frame and not next to the objects.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, CTable, GameWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#include "records.h"

struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
extern void GameWin_ResizePort(GameWin *w, void *rect);
extern void MSLockGraf(GRPort *port);

/* same object: 4-byte `push cs; call ` */
extern "C" void show_window_1050_8bc8(GameWin *w);

void show_backdrop(BackdropWin *w, register int which)
{
    GRPort port;
    CTable ct;
    RECT dst;
    RECT src;

    GRPort_LoadPicture(&port, 0xc9, 0x28, 0x60, (struct logpal *)&ct);
    SetRect(&dst, 0, 0, 0x280, 0x1e0);
    if (which == 1)
        SetRect(&src, 0, 0, 0x280, 0x1e0);
    else if (which == 2)
        SetRect(&src, 0x280, 0, 0x500, 0x1e0);
    if (w->shown_half == -1) {
        GameWin_ResizePort((GameWin *)w, &dst);
        MSLockGraf((GRPort *)w);
        show_window_1050_8bc8((GameWin *)w);
    }
    w->shown_half = which;
    GRPort_CopyToPort(&port, (GRPort *)w, &src, &dst);
    copy_all_to_screen_1050_843b((GameWin *)w);
}

