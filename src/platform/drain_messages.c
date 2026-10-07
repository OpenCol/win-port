/* drain_messages: candidate for 1078:112f (44 bytes).  Verdict: RELOCATABLE.
 *
 * Removes every pending message from the queue.
 */

#include "colonization.h"

typedef struct { unsigned hwnd, message, wParam; long lParam; unsigned long time;
                 int pt, y; } MSG;   /* 18 bytes, and every member is
   WINDOWS.H's: HWND and UINT and WPARAM are `unsigned`, LPARAM is `signed long`,
   DWORD is `unsigned long`, POINT is two `int` -- kept under the corpus's own names
   `pt` and `y` for those two offsets, because `check_field_names` holds one name
   per offset and WINDOWS.H's `x` is not the name this tree already used. The four
   files that declare MSG
   had four different spellings and `analysis/signedness.json` could not test two
   of them; nothing here READS a member, so the types were free and the header the
   game was compiled against decides them (2026-09-24). */
extern int PeekMessage(MSG *m, unsigned hwnd, unsigned lo, unsigned hi, unsigned remove);

void drain_messages(void)
{
    MSG msg;

    while (PeekMessage(&msg, 0, 0, 0, 1) != 0)
        ;
}
