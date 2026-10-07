/* port_validate_rect_1038_e582: candidate for 1038:e582 (39 bytes).  Verdict: RELOCATABLE.
 *
 * The copy in segment 1038 of port_validate_rect (1048:e6a9). Validates a rectangle of the window a port belongs to. The 32-bit field at +6
 * is a POINTER to that window's `MSWin` object, not a handle: `MSValidRect` takes
 * `MSWin *w` and reads the handle from its own +2. The same 39 bytes occur in fifteen segments, one per module, beside
 * copy_rect_to_screen.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

extern void MSValidRect(void *win, void *rect);

void port_validate_rect_1038_e582(DialogWin *p, void *rect)
{
    MSValidRect((MSWin *)p->win, (RECT *)rect);
}
