/* call_handler_slot_56 -- shipped at 1070:9edb.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_56.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x38]; void ( *handler)(void); } HandlerTable;

void call_handler_slot_56(HandlerTable *t)
{
    if (t->handler != 0)
        (*t->handler)();
}
