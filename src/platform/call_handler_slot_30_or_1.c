/* call_handler_slot_30_or_1 -- shipped at 1070:9aa0.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_30_or_1.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad0[2]; col_i16 id; char pad[0x1e - 4]; col_i16 ( *handler)(col_i16 id, col_i16 a); } HandlerTable;

col_i16 call_handler_slot_30_or_1(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        return (*t->handler)(t->id, a);
    else
        return 1;
}
