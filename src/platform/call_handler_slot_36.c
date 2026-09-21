/* call_handler_slot_36 -- shipped at 1070:9e18.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_36.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x24]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_36(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
