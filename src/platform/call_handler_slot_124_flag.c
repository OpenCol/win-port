/* call_handler_slot_124_flag -- shipped at 1070:9bd9.
 *
 * Ported by tools/port.py from matched/platform/call_handler_slot_124_flag.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x7c]; void ( *handler)(col_i16 a, col_i16 b); } HandlerTable;

char call_handler_slot_124_flag(HandlerTable *t, col_i16 a, col_i16 b)
{
    if (t->handler != 0) {
        (*t->handler)(a, b);
        return 1;
    } else {
        return 0;
    }
}
