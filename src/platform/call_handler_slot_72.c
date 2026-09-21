/* call_handler_slot_72 -- shipped at 1070:9f27.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_72.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x48]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_72(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
