/* call_handler_slot_192 -- shipped at 1060:ae50.
 *
 * Ported by tools/port.py from matched/game/call_handler_slot_192.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0xc0]; void ( *handler)(void); } HandlerObj;

void call_handler_slot_192(HandlerObj *o)
{
    if (o->handler != 0)
        (*o->handler)();
}
