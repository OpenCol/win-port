/* dlg_control_init_1068_e478 -- shipped at 1068:e478.
 *
 * Ported by tools/port.py from matched/gfx/dlg_control_init_1068_e478.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1a bytes */
extern void register_control(void *win, DlgControl *c);

void dlg_control_init_1068_e478(DlgControl *o, void *win, col_i16 x, col_i16 y, RECT *r)
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
