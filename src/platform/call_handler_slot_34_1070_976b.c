/* call_handler_slot_34_1070_976b -- shipped at 1070:976b.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_34_1070_976b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x22]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_34_1070_976b(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
