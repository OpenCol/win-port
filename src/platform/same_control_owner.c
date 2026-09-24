/* same_control_owner -- shipped at 1070:5090.
 *
 * Ported by tools/port.py from matched/platform/same_control_owner.c.
 *
 * The file's own Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Rec * object_from_hwnd(col_u16 h);
col_i16 hwnd_is_ms_control(col_u16 h);

/* same object: 4-byte `push cs; call ` */



col_i16 same_control_owner(register col_u16 a, register col_u16 b)
{
    Rec *oa;
    void *pa;
    Rec *ob;
    void *pb;

    if (hwnd_is_ms_control(a) == 0 || hwnd_is_ms_control(b) == 0)
        return 0;
    if (a == b)
        return 1;
    oa = object_from_hwnd(a);
    ob = object_from_hwnd(b);
    pa = (*((void * *)((unsigned char *)(oa) + 0x2)));
    pb = (*((void * *)((unsigned char *)(ob) + 0x2)));
    if (pa == pb)
        return 1;
    return 0;
}
