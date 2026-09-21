/* call_handler_slot_32 -- shipped at 1070:9da6.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_32.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x20]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_32(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
