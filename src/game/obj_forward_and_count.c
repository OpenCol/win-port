/* obj_forward_and_count -- shipped at 1050:86ac.
 *
 * Ported by tools/port.py from matched/game/obj_forward_and_count.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void listbox_add_string(col_i16 h, void *p);

void obj_forward_and_count(Obj *o, void *p)
{
    listbox_add_string(o->handle, p);
    o->count++;
}
