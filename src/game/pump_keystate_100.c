/* pump_keystate_100 -- shipped at 1060:a7df.
 *
 * Ported by tools/port.py from matched/game/pump_keystate_100.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 message_pump_with_keystate(col_i16 kind);

col_i16 pump_keystate_100(void *unused1)
{
    return message_pump_with_keystate(0x100);
}
