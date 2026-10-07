/* port_validate_rect: candidate for 1048:e6a9 (39 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * Validates a rectangle of the window a port belongs to. The 32-bit field at +6 is a POINTER
 * to that window's `MSWin` object, not a handle: `MSValidRect` takes `MSWin *w` and reads
 * the handle from its own +2. The same 39 bytes occur in fifteen segments, one per module, beside
 * copy_rect_to_screen.
 *
 * AN OPEN QUESTION about the same field. `win_release_1020_ac61` hands `o->port` --
 * this same record -- to `dialog_dispose_window_1020_af04`, which reads +6 as a `Menu` and tears it
 * down. `Menu` and `MSWin` share their first two words (`handle` at +0, `hwnd` at +2), so
 * both readings agree on the LAYOUT of what +6 points at and disagree only on what to call
 * it. `MSValidRect`'s own signature is why this copy says `win`. Unresolved.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void MSValidRect(void *win, void *rect);

void port_validate_rect(DialogWin *p, void *rect)
{
    MSValidRect((MSWin *)p->win, (RECT *)rect);
}
