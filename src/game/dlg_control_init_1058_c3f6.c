/* dlg_control_init_1058_c3f6 -- shipped at 1058:c3f6.
 *
 * Ported by tools/port.py from matched/game/dlg_control_init_1058_c3f6.c.
 *
 * The file's own DlgControl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1a bytes */
extern void register_control(void *win, DlgControl *c);

void dlg_control_init_1058_c3f6(DlgControl *o, void *win, col_i16 kind, col_i16 id, RECT *r)
{
    (*((void * *)((unsigned char *)(o) + 0x4))) = win;
    (*((col_i16 *)((unsigned char *)(o) + 0x0))) = kind;
    (*((col_i16 *)((unsigned char *)(o) + 0x2))) = id;
    (*((RECT *)((unsigned char *)(o) + 0x8))) = *r;
    (*((void * *)((unsigned char *)(o) + 0x12))) = 0;
    o->hwnd = 0;
    (*((char *)((unsigned char *)(o) + 0x16))) = 0;
    register_control(win, o);
}
