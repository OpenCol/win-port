/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* get_window_text: candidate for 1070:5a90 (46 bytes).  Verdict: RELOCATABLE.
 *
 * WM_GETTEXT into the buffer with WM_GETTEXTLENGTH + 1 as the size.
 *
 * `buf` is a pointer, as callers pass it, cast to the message's long (2026-10-02).
 */

#include "colonization.h"

void get_window_text(int h, char *buf)
{
    SendMessage(h, 0xd, SendMessage(h, 0xe, 0, 0L) + 1, (intptr_t)buf);
}
