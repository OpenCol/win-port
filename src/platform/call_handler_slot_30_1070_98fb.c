/* call_handler_slot_30_1070_98fb -- shipped at 1070:98fb.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_30_1070_98fb.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x1e]; void ( *handler)(col_i16 a); } HandlerTable;

void call_handler_slot_30_1070_98fb(HandlerTable *t, col_i16 a)
{
    if (t->handler != 0)
        (*t->handler)(a);
}
