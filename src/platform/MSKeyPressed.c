/* MSKeyPressed: candidate for 1078:2be1 (176 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * kind 0x200: is Control down; 0x100: is Shift down; 0x400: pump the queue, reporting 1 if
 * a keyboard message (0x104..0x106) was seen and dispatching everything else.
 *
 * RENAMED 2026-09-27 from message_pump_with_keystate: the Mac build's MSKeyPressed, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Answers whether Ctrl (0x200) or Shift (0x100) is down, or whether any key message is queued (0x400); Mac caller Screen::ShiftKeyPressed, 1 arg, ratio 0.64.
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
extern int TranslateMessage(MSG *m);
extern long DispatchMessage(MSG *m);

int MSKeyPressed(int kind)
{
    char hit = 0;
    MSG m;

    if (kind == 0x200) {
        hit = (GetKeyState(0x11) & 0x8000) ? 1 : 0;
    } else if (kind == 0x100) {
        hit = (GetKeyState(0x10) & 0x8000) ? 1 : 0;
    } else if (kind == 0x400) {
        while (PeekMessage(&m, 0, 0, 0, 1)) {
            if (m.message == 0x106 || m.message == 0x104 || m.message == 0x105) {
                hit = 1;
            } else {
                TranslateMessage(&m);
                DispatchMessage(&m);
            }
        }
    }
    return hit;
}
