/* control_message_handler: candidate for 1070:07c5 (187 bytes).  Verdict: RELOCATABLE.
 *
 * Window procedure of MSControlClass windows: key messages (0x100..0x102) are forwarded to
 * the parent, WM_PAINT renders through wing_child_paint, WM_DESTROY disposes the control
 * block and defers to DefWindowProc, everything else is DefWindowProc.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

long control_message_handler(unsigned hwnd, unsigned msg, unsigned wparam, long lparam)
{
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
        SendMessage(GetParent(hwnd), msg, wparam, lparam);
        break;
    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        wing_child_paint(&ps, hwnd);
        EndPaint(hwnd, &ps);
        break;
    case WM_DESTROY:
        DisposeCtrlBlock(hwnd);
        return DefWindowProc(hwnd, msg, wparam, lparam);
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
