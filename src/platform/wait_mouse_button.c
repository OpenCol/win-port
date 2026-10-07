/* wait_mouse_button: candidate for 1078:2b35 (172 bytes).  Verdict: RELOCATABLE.
 *
 * Watches the mouse-button messages (client 0x200..0x209 and non-client
 * 0xa2..0xa5) for the window, or the whole queue for a null window: with
 * `left` set a LEFT button-up (0x202/0xa2, WM_LBUTTONUP/WM_NCLBUTTONUP) is the hit, otherwise a
 * RIGHT button-up (0x205/0xa5, WM_RBUTTONUP/WM_NCRBUTTONUP) -- both are releases; the flag picks
 * the button (it was named `up`, and this said right-button down, until 2026-09-27);
 * `remove` takes the messages off the queue and `wait`
 * keeps polling until a hit.  Returns whether one was seen.
 *
 * REGISTER ALLOCATION (rule 40). The first-declared local is auto-enregistered into DI;
 * an explicit `register` promotes a local to SI, displacing an enregistered parameter; a
 * local that is neither first nor `register` goes to the stack. This file needs the
 * count first and plain, which is the combination that was missing when it was parked.
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
#include "records.h"
extern int PeekMessage(MSG *m, unsigned hwnd, unsigned lo, unsigned hi, unsigned remove);

int wait_mouse_button(MSWin *win, char left, char remove, char wait)
{
    int hwnd;
    register int rem;
    char r = 0;
    MSG m1;
    MSG m2;

    if (win != 0)
        hwnd = win->hwnd;
    else
        hwnd = 0;
    if (remove != 0)
        rem = 1;
    else
        rem = 0;
    do {
        while (PeekMessage(&m1, hwnd, 0x200, 0x209, rem) != 0 || PeekMessage(&m2, hwnd, 0xa2, 0xa5, rem) != 0) {
            if (left != 0 && (m1.message == 0x202 || m2.message == 0xa2)
                || left == 0 && (m1.message == 0x205 || m2.message == 0xa5)) {
                r = 1;
                wait = 0;
            }
        }
    } while (wait != 0);
    return r;
}
