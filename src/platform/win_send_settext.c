/* win_send_settext: candidate for 1070:5abe (30 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, WM_SETTEXT, 0, text)` -- the message form of setting a window's text,
 * where `win_set_text` (1070:656f) calls `SetWindowText` for the same effect. Named after the
 * message it sends, the way `win_send_415`, the next function 30 bytes later, is.
 *
 * Four pushes in `` order: the handle from [bp+6], the message, a zero wparam, and the
 * caller's four-byte lparam from [bp+8] as one `PUSH DWORD` rather than two word pushes.
 *
 * Its `text` is a pointer, as every caller passes one (2026-10-02; it was a `long`, the same pushes).
 */

#include "colonization.h"

#include "w16const.h"

void win_send_settext(unsigned hwnd, void *text)
{
    SendMessage(hwnd, WM_SETTEXT, 0, (long)text);
}
