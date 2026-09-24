/* pump_keystate_200 -- shipped at 1060:a7fc.
 *
 * Ported by tools/port.py from matched/game/pump_keystate_200.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 message_pump_with_keystate(col_i16 kind);

col_i16 pump_keystate_200(void)
{
    return message_pump_with_keystate(0x200);
}
