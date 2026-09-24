/* listbox_init -- shipped at 1050:865a.
 *
 * Ported by tools/port.py from matched/game/listbox_init.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1050_9418(void *o, void *win, col_i16 kind, col_i16 id, RECT *r);

typedef struct { col_i16 kind, id; void *win; RECT rect; col_i16 hwnd; void *next; char f16; char pad[3];
                 col_i16 f1a; char rest[0x22 - 0x1c]; } SListBox; /* 0x22 bytes */
extern col_u16 register_MSListBoxClass(void *r, void *ctl, col_i16 visible);

/* same object: 4-byte `push cs; call ` */


void listbox_init(SListBox *c, void *win, col_i16 id, RECT *r)
{
    dlg_control_init_1050_9418(c, win, 7, id, r);
    c->hwnd = register_MSListBoxClass(r, c, 1);
    c->f1a = 0;
}
