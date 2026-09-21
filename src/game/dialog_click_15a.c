/* dialog_click_15a -- shipped at 1030:08c4.
 *
 * Ported by tools/port.py from matched/game/dialog_click_15a.c.
 *
 * The file's own Owner dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: done -> done_15a, ok -> ok_15c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);

void dialog_click_15a(col_i16 code)
{
    Owner *o;
    char *w = current_window();

    o = (Owner *)(w != 0 ? w - 0x26 : w);
    o->done_15a = 1;
    if (code == 0x65)
        o->ok_15c = 1;
}
