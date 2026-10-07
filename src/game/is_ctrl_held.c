/* is_ctrl_held: candidate for 1060:a7fc (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * `MSKeyPressed(0x200)`, one of THREE identical wrappers back to back at
 * 1060:a7c2, a7df and a7fc passing 0x400, 0x100 and 0x200 -- three single bits, but
 * MSKeyPressed compares `kind` for equality -- 0x200 Ctrl down, 0x100 Shift down, 0x400 a key message queued -- and each wrapper names one.
 *
 * RENAMED 2026-09-26 from pump_keystate_200: whether Ctrl is held: MSKeyPressed(0x200) is
 * GetKeyState(VK_CONTROL) & 0x8000.
 */

#include "colonization.h"

extern int MSKeyPressed(int kind);

int is_ctrl_held(void)
{
    return MSKeyPressed(0x200);
}
