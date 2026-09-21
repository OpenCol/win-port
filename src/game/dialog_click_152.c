/* dialog_click_152 -- shipped at 1030:0000.
 *
 * Ported by tools/port.py from matched/game/dialog_click_152.c.
 *
 * The file's own Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);

void dialog_click_152(col_i16 code)
{
    Owner *o;
    char *w = current_window();

    o = (Owner *)(w != 0 ? w - 0x26 : w);
    o->done = 1;
    if (code == 0x65)
        o->ok = 1;
}
