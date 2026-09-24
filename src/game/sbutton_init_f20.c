/* sbutton_init_f20 -- shipped at 1010:eabb.
 *
 * Ported by tools/port.py from matched/game/sbutton_init_f20.c.
 *
 * The file's own DlgControl, SButton dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1010_edf9(DlgControl *o, void *win, col_i16 x, col_i16 y, RECT *r);
void sbutton_init_1010_eeac(SButton *o, void *win, col_i16 y, RECT *r, char *title);

/* 0x1a bytes */
/* 0x22 bytes */
extern void register_control(void *win, DlgControl *c);
extern col_i16 MSInitButton(RECT *r, SButton *b, char *title, col_i16 a);





void sbutton_init_f20(SButton *o, void *win, col_i16 y, RECT *r, col_i16 f20, char *title)
{
    (*((col_i16 *)((unsigned char *)(o) + 0x34))) = f20;
    sbutton_init_1010_eeac(o, win, y, r, title);
}
