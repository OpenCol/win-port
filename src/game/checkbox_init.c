/* checkbox_init -- shipped at 1050:8718.
 *
 * Ported by tools/port.py from matched/game/checkbox_init.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1050_9418(void *o, void *win, col_i16 kind, col_i16 id,
                                    RECT *r);

typedef struct { col_i16 kind, id; void *win; RECT rect; col_i16 hwnd; void *next; char f16; char pad[3];
                 col_i32 f1a; col_i16 f1e; col_i16 f20; } CheckBox;
extern void text_row_rect(char *s, col_i16 x, col_i16 y, RECT *r);
extern col_u16 MSInitCheck(RECT *r, void *ctl, const char *text, col_i16 visible);

/* same object: 4-byte `push cs; call ` */


void checkbox_init(CheckBox *c, void *win, col_i16 id, col_i16 rx, col_i16 ry, char *text)
{
    RECT r;

    text_row_rect(text, rx, ry, &r);
    dlg_control_init_1050_9418(c, win, 2, id, &r);
    c->f1a = 0L;
    c->f20 = 1;
    c->f1e = 0;
    c->hwnd = MSInitCheck(&r, c, text, 1);
}
