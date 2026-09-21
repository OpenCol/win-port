/* call_handler_slot_128_flag -- shipped at 1070:9ba2.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_128_flag.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x80]; void ( *handler)(col_i32 e); } HandlerTable;

char call_handler_slot_128_flag(HandlerTable *t, col_i32 e)
{
    if (t->handler != 0) {
        (*t->handler)(e);
        return 1;
    } else {
        return 0;
    }
}
