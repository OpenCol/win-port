/* dialog_click_19e -- shipped at 1030:11ea.
 *
 * Ported by tools/port.py from matched/game/dialog_click_19e.c.
 *
 * The file's own Owner dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: done -> done_19e, ok -> ok_1a0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);

void dialog_click_19e(col_i16 code)
{
    Owner *o;
    char *w = current_window();

    o = (Owner *)(w != 0 ? w - 0x26 : w);
    o->done_19e = 1;
    if (code == 0x65)
        o->ok_1a0 = 1;
}
