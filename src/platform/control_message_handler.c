/* control_message_handler -- shipped at 1070:07c5.
 *
 * Ported by tools/port.py from matched/platform/control_message_handler.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void wing_child_paint(void *ps, col_u16 hwnd);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal EndPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_i32 pascal DefWindowProc(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);
extern col_i32 pascal SendMessage(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam);
extern col_u16 pascal BeginPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_u16 pascal GetParent(col_u16 hwnd);
#endif

#include "w16const.h"





/* same object: 4-byte `push cs; call ` (rule 47) */

extern void DisposeCtrlBlock(col_u16 hwnd);

col_i32 pascal control_message_handler(col_u16 hwnd, col_u16 msg, col_u16 wparam, col_i32 lparam)
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
