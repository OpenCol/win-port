/* call_handler_slot_30 -- shipped at 1070:9a0b.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_30.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x1e]; void ( *handler)(col_i16 a, col_i16 b); } HandlerTable;

void call_handler_slot_30(HandlerTable *t, col_i16 a, col_i16 b)
{
    if (t->handler != 0)
        (*t->handler)(a, b);
}
