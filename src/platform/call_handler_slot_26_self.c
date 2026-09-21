/* call_handler_slot_26_self -- shipped at 1070:a0b7.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_26_self.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad0[2]; col_i16 id; char pad[0x1a - 4]; void ( *handler)(col_i16 id); } HandlerTable;

void call_handler_slot_26_self(HandlerTable *t)
{
    if (t->handler != 0)
        (*t->handler)(t->id);
}
