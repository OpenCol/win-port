/* call_handler_slot_16 -- shipped at 1070:9cdb.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_16.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x10]; void ( *handler)(col_i16 a, col_i16 b); } HandlerTable;

void call_handler_slot_16(HandlerTable *t, col_i16 a, col_i16 b)
{
    if (t->handler != 0)
        (*t->handler)(a, b);
}
