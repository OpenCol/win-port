/* call_handler_slot_34 -- shipped at 1070:9a34.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_34.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x22]; void ( *handler)(col_i16 a, col_i16 b); } HandlerTable;

void call_handler_slot_34(HandlerTable *t, col_i16 a, col_i16 b)
{
    if (t->handler != 0)
        (*t->handler)(a, b);
}
