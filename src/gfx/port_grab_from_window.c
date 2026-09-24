/* port_grab_from_window -- shipped at 1068:b65b.
 *
 * Ported by tools/port.py from matched/gfx/port_grab_from_window.c.
 *
 * The file's own Win, Obj, Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
RECT * ptr_plus_16(Port *p);
Win * GRPort_GetWin_1068_d91d(Port *p);
void * window_of_1068_d72a(Obj *o);

extern void wing_grab_from_window(Win *dst, void *win, RECT *r);

/* same object: 4-byte `push cs; call ` */




void port_grab_from_window(Port *p)
{
    wing_grab_from_window(GRPort_GetWin_1068_d91d(p),
                                    window_of_1068_d72a(&(*((Obj *)((unsigned char *)(p) + 0x26)))),
                                    ptr_plus_16(p));
}
