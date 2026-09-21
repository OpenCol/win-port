/* call_handler_slot_26_1070_99e2 -- shipped at 1070:99e2.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_26_1070_99e2.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x1a]; void ( *handler)(col_i16 a, col_i16 b); } HandlerTable;

void call_handler_slot_26_1070_99e2(HandlerTable *t, col_i16 a, col_i16 b)
{
    if (t->handler != 0)
        (*t->handler)(a, b);
}
