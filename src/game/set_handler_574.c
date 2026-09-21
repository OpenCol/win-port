/* set_handler_574 -- shipped at 1010:2b0f.
 *
 * Ported by tools/port.py from matched/game/set_handler_574.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_handler_574(Obj *o, void *h)
{
    (*((void * *)((unsigned char *)(o) + 0x574))) = h;
}
