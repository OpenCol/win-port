/* call_handler_slot_88 -- shipped at 1070:a00c.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_88.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x58]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_88(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
