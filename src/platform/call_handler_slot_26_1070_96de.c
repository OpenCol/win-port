/* call_handler_slot_26_1070_96de -- shipped at 1070:96de.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_26_1070_96de.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x1a]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_26_1070_96de(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
