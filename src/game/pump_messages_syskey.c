/* pump_messages_syskey: candidate for 1060:a7c2 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * `MSKeyPressed(0x400)`, one of THREE identical wrappers back to back at
 * 1060:a7c2, a7df and a7fc passing 0x400, 0x100 and 0x200 -- three selectors, not a mask:
 * MSKeyPressed tests kind with `==`; 0x100 reads the Shift state, 0x200 the Ctrl state, and only 0x400 pumps the queue.
 *
 * RENAMED 2026-09-26 from pump_keystate_400: MSKeyPressed(0x400): drains the queue, dispatching
 * everything except WM_SYSKEYDOWN/WM_SYSKEYUP/WM_SYSCHAR (0x104..0x106), and reports whether one
 * of those was seen.
 */

#include "colonization.h"

extern int MSKeyPressed(int kind);

int pump_messages_syskey(void)
{
    return MSKeyPressed(0x400);
}
