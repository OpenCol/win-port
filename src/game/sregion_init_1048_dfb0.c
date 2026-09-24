/* sregion_init_1048_dfb0 -- shipped at 1048:dfb0.
 *
 * Ported by tools/port.py from matched/game/sregion_init_1048_dfb0.c.
 *
 * The file's own DlgControl, SRegion dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1048_e633(DlgControl *o, void *win, col_i16 kind, col_i16 id, RECT *r);

/* 0x1a bytes */
/* 0x2e bytes */
extern void register_control(void *win, DlgControl *c);
extern col_i16 MSInitRegion(RECT *r, DlgControl *b);



void sregion_init_1048_dfb0(SRegion *o, void *win, col_i16 id, RECT *r)
{
    dlg_control_init_1048_e633(&(*((DlgControl *)((unsigned char *)(o) + 0x0))), win, 1, id, r);
    (*((col_i32 *)((unsigned char *)(o) + 0x2e))) = 0;
    (*((col_i32 *)((unsigned char *)(o) + 0x32))) = 0;
    (*((col_i32 *)((unsigned char *)(o) + 0x36))) = 0;
    (*((col_i32 *)((unsigned char *)(o) + 0x3a))) = 0;
    (*((col_i32 *)((unsigned char *)(o) + 0x3e))) = 0;
    (*((DlgControl *)((unsigned char *)(o) + 0x0))).hwnd = MSInitRegion(r, &(*((DlgControl *)((unsigned char *)(o) + 0x0))));
}
