/* Screen_ShiftKeyPressed: candidate for 1060:a7df (29 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * `MSKeyPressed(0x100)`, one of THREE identical wrappers back to back at
 * 1060:a7c2, a7df and a7fc passing 0x400, 0x100 and 0x200 -- three selector values, and
 * MSKeyPressed tests the parameter by equality (0x200 Control down, 0x100 Shift down, 0x400 a key
 * message queued), so each wrapper names one query.
 *
 * RENAMED 2026-09-26 from pump_keystate_100: whether Shift is held: MSKeyPressed(0x100) is
 * GetKeyState(VK_SHIFT) & 0x8000. shift_key asks it.
 *
 * RENAMED 2026-09-26 from is_shift_held: the Mac build's Screen::ShiftKeyPressed(void): its one parameter is the unread `this`; it asks the message pump for the Shift bit. The width matches.
 */

#include "colonization.h"

#include "classes.h"
extern int MSKeyPressed(int kind);

int Screen_ShiftKeyPressed(Screen *unused1)
{
    return MSKeyPressed(0x100);
}
