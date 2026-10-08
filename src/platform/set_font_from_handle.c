/* PORT: the font record's handle is a 16-bit token followed by `owned`: read as u16, not int (4 bytes here) (W4). */
/* set_font_from_handle: candidate for 1070:5452 (68 bytes).  Verdict: RELOCATABLE.
 *
 * WM_SETFONT on the window with the font handle read through the locked
 * memory handle.
 */

#include "colonization.h"

void set_font_from_handle(int hwnd, int hmem)
{
    u16 *p;

    p = (u16 *)MSHLock(hmem);
    SendMessage(hwnd, 0x30, *p, 0L);
    MSHUnlock(hmem);
}
