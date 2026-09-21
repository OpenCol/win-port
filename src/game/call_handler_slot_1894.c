/* call_handler_slot_1894 -- shipped at 1048:d885.
 *
 * Ported by tools/port.py from matched/game/call_handler_slot_1894.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x766]; void ( *handler)(col_i16 a); } HandlerObj;

void call_handler_slot_1894(HandlerObj *o, col_i16 a)
{
    if (o->handler != 0)
        (*o->handler)(a);
}
