/* GameWin_ResizePort: candidate for 1060:a281 (93 bytes, 4 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GameWin::ResizePort(RECT*)
 * name-source: sibling-build
 *
 * Rebuilds the window's port over `rect` (GRPort_ResizePort); when that returns 1, sets the
 * client size of the dialog at +0x26 to the rect's width and height and returns 1; else 0.
 *
 * RENAMED 2026-09-26 from win_click_to_port: the Mac build's GameWin::ResizePort(Rect*): the name audit found it rebuilds the window's port over a RECT and resizes the client area to it (no click, no event), and the uneven-gap layout had already placed ResizePort here, between GameWin_Redraw and GameWin_Resize on both sides -- held then only because the old name said click. The arity is (this, rect) on both sides.
 *
 * A METHOD since 2026-10-02: GameWin::ResizePort, the one virtual (every module's GameWin vtable
 * names it). It was a C function over byte views; the base call is the qualified
 * GRPort::ResizePort, the dialog part `&(DialogWin &)*this`.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void win_set_client_size_1060_aecf(DialogWin *port, int x, int y);
int GameWin::ResizePort(RECT *rect)
{
    if (GRPort::ResizePort(rect) == 1) {
        win_set_client_size_1060_aecf(&(DialogWin &)*this, rect_width_1060_ae90(rect), rect_height_1060_aeaf(rect));
        return 1;
    } else
        return 0;
}
