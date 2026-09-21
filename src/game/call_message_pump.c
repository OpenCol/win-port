/* call_message_pump -- shipped at 1008:d157.
 *
 * Ported by tools/port.py from matched/game/call_message_pump.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSDoTask(void);

void call_message_pump(void)
{
    MSDoTask();
    return;
}
