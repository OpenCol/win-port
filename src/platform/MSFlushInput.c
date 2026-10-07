/* MSFlushInput: candidate for 1078:10e7 (72 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Drains the queue of mouse (0x200..0x209) and then keyboard (0x100..0x108) messages,
 * discarding them.
 *
 * RENAMED 2026-09-26 from peek_message_only: It calls PeekMessage with PM_REMOVE in a loop over WM_MOUSEFIRST..0x209 and then WM_KEYFIRST..0x108, removing and discarding every pending mouse and keyboard message. 'peek ... only' reads as a look at the queue that removes nothing, which is the opposite of what the body does. It flushes pending input, and PediaWin_ShowModal, among others, calls it before a modal loop. The Mac build's macui has MSFlushInput(void) with the same role and arity. I left that name out because role and arity are the only evidence. Note: four one-line forwarders are named peek_message_only_<seg>_<off> (1008:cefc, 1050:8d10, 1058:bba6, 1060:6713) and would follow.
 *
 * RENAMED 2026-09-27 from flush_input_messages: the Mac build's MSFlushInput(void), bound by call-sequence alignment (tools/mac_bind.py): the body drains the mouse (0x200..0x209) and keyboard (0x100..0x108) messages; the forwarders carry its name with their address.
 */

#include "colonization.h"

typedef struct { unsigned hwnd, message, wParam; long lParam; unsigned long time;
                 int pt, y; } MSG;   /* 18 bytes; LPARAM is `signed long` and DWORD `unsigned long`,
   as in WINDOWS.H and the four other files that declare MSG */
extern int PeekMessage(MSG *m, unsigned hwnd, unsigned lo, unsigned hi, unsigned remove);

void MSFlushInput(void)
{
    MSG m;

    while (PeekMessage(&m, 0, 0x200, 0x209, 1))
        ;
    while (PeekMessage(&m, 0, 0x100, 0x108, 1))
        ;
}
