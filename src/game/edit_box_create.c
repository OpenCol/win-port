/* edit_box_create: candidate for 1060:9076 (100 bytes).  Verdict: RELOCATABLE.
 * symbol: edit_box_create
 *
 * Like SEditBox_Init with a style word passed on to
 * edit_box_init_1060_a9bc (-called, defined in-file as a stub).
 *
 * RENAMED 2026-09-27 from edit_box_create_1060_9076: its family's other copy took a Mac name in this batch, so this is now the family's only member and drops its address suffix (check_naming: a singleton carries no suffix).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern void edit_box_init_1060_a9bc(SEditBox *edit, void *win, int id, RECT *r, void *text, int style);

void edit_box_create(SEditBox *edit, void *win, int id, int x, int y, int w, void *text, int style)
{
    RECT r;

    edit->on_change = 0;
    SetRect(&r, x, y, x + w, y + 0x1e);
    edit_box_init_1060_a9bc(edit, win, id, &r, text, style);
}
