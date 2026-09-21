/* call_handler_slot_1890 -- shipped at 1048:d941.
 *
 * Ported by tools/port.py from matched/game/call_handler_slot_1890.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x762]; void ( *handler)(void); } HandlerObj;

void call_handler_slot_1890(HandlerObj *o)
{
    if (o->handler != 0)
        (*o->handler)();
}
