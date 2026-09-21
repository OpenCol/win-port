/* call_handler_slot_48_result -- shipped at 1070:9efb.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_48_result.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x30]; col_i16 ( *handler)(void); } HandlerTable;

col_i16 call_handler_slot_48_result(HandlerTable *t)
{
    if (t->handler != 0)
        return (*t->handler)();
    else
        return 0;
}
