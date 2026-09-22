/* control_is_current -- shipped at 1070:502b.
 *
 * Ported by tools/port.py from matched/platform/control_is_current.c.
 *
 * The file's own Obj, Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Obj * object_from_hwnd(col_u16 h);
col_i16 get_w_2a_1070_99ac(void *r);

/* same object: 4-byte `push cs; call ` */



col_i16 control_is_current(col_u16 hwnd)
{
    Obj *o;
    void *ctl;
    Owner *owner;

    o = object_from_hwnd(hwnd);
    ctl = (*((void * *)((unsigned char *)(o) + 0x2)));
    owner = (*((void * *)((unsigned char *)((Owner *)o) + 0x14)));
    if (get_w_2a_1070_99ac(ctl) == owner->sel)
        return 1;
    return 0;
}
