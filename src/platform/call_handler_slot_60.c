/* call_handler_slot_60 -- shipped at 1070:9e3e.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_60.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x3c]; void ( *handler)(void); } HandlerTable;

void call_handler_slot_60(HandlerTable *t)
{
    if (t->handler != 0)
        (*t->handler)();
}
