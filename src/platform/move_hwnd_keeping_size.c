/* move_hwnd_keeping_size: candidate for 1070:0b8b (88 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Moves a window to (x, y) without resizing it: ask Windows for the current rectangle, then
 * `MoveWindow` with the same width and height back. A null HWND returns without asking.
 *
 * The width and height come from `rect_width_1070_962a` and `rect_height_1070_9649`, the same pair
 * `MSInitRegion` uses to size a window at creation -- so the RECT that `GetWindowRect` fills
 * and the Region record those helpers normally read have the same first four words.
 *
 * `GetWindowRect` and `MoveWindow` are `` and leave no `ADD SP`; the two size helpers
 * are the module's own and do.
 */

#include "colonization.h"

#include "w16types.h"

/* same object: 4-byte `push cs; call ` */

void move_hwnd_keeping_size(register unsigned hwnd, int x, int y)
{
    RECT r;

    if (hwnd == 0)
        return;
    GetWindowRect(hwnd, &r);
    MoveWindow(hwnd, x, y, rect_width_1070_962a(&r), rect_height_1070_9649(&r), 1);
}
