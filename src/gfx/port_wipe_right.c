/* port_wipe_right -- shipped at 1068:b463.
 *
 * Ported by tools/port.py from matched/gfx/port_wipe_right.c.
 *
 * The file's own Win, Obj, GRPort, Screen dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Win * GRPort_GetWin_1068_d91d(GRPort *p);
void * window_of_1068_d72a(Obj *o);

extern void wing_wipe_right(Win *a, Win *b, void *port, RECT *r, col_i16 step);

/* same object: 4-byte `push cs; call ` */



void port_wipe_right(Screen *s, col_i16 step, RECT *r)
{
    wing_wipe_right(GRPort_GetWin_1068_d91d(&(*((GRPort *)((unsigned char *)(s) + 0x0)))),
                           GRPort_GetWin_1068_d91d(&(*((GRPort *)((unsigned char *)(s) + 0x90a)))),
                           window_of_1068_d72a(&(*((Obj *)((unsigned char *)(s) + 0x26)))), r, step);
}
