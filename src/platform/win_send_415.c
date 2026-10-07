/* win_send_415: candidate for 1070:5adc (26 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, 0x415, wparam, 0)` -- 0x415 is WM_USER + 0x15, which WINDOWS.H defines as
 * EM_LIMITTEXT: the edit control's character limit (MSSetEBLimit sends the same message).
 * (Corrected 2026-09-27: this said it was the GUI library's own message, not a Windows one.)
 *
 * `MSSetEBLimit` is the caller that says what one use of it is: it passes an edit
 * control's handle and a character count (the text limit).
 */

#include "colonization.h"

void win_send_415(unsigned hwnd, unsigned wp)
{
    SendMessage(hwnd, 0x415, wp, 0L);
}
