/* sbutton_init_1010_eeac -- shipped at 1010:eeac.
 *
 * Ported by tools/port.py from matched/game/sbutton_init_1010_eeac.c.
 *
 * The file's own DlgControl, SButton dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1010_edf9(DlgControl *o, void *win, col_i16 kind, col_i16 id, RECT *r);

/* 0x1a bytes */
/* 0x22 bytes */
extern void register_control(void *win, DlgControl *c);
extern col_i16 MSInitButton(RECT *r, SButton *b, char *title, col_i16 a);



void sbutton_init_1010_eeac(SButton *o, void *win, col_i16 id, RECT *r, char *title)
{
    dlg_control_init_1010_edf9(&(*((DlgControl *)((unsigned char *)(o) + 0x0))), win, 6, id, r);
    (*((col_i32 *)((unsigned char *)(o) + 0x30))) = 0;
    (*((DlgControl *)((unsigned char *)(o) + 0x0))).hwnd = MSInitButton(r, o, title, 1);
    (*((col_i16 *)((unsigned char *)(o) + 0x2e))) = 1;
}
