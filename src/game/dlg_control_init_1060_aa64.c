/* dlg_control_init_1060_aa64 -- shipped at 1060:aa64.
 *
 * Ported by tools/port.py from matched/game/dlg_control_init_1060_aa64.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void register_control(void *win, DlgControl *c);

/* 0x1a bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */

void dlg_control_init_1060_aa64(DlgControl *o, void *win, col_i16 x, col_i16 y, RECT *r)
{
    (*((void * *)((unsigned char *)(o) + 0x4))) = win;
    (*((col_i16 *)((unsigned char *)(o) + 0x0))) = x;
    (*((col_i16 *)((unsigned char *)(o) + 0x2))) = y;
    (*((RECT *)((unsigned char *)(o) + 0x8))) = *r;
    (*((col_i32 *)((unsigned char *)(o) + 0x12))) = 0;
    o->hwnd = 0;
    (*((char *)((unsigned char *)(o) + 0x16))) = 0;
    register_control(win, o);
}
