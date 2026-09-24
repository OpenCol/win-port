/* pedia_key -- shipped at 1060:1fce.
 *
 * Ported by tools/port.py from matched/game/pedia_key.c.
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern char * current_window(void);
extern void win_close_1060_2a47(PediaWin *o);
extern void keyboard_accelerator_handler(col_i16 key);

void pedia_key(col_i16 key)
{
    PediaWin *o;
    char *w = current_window();

    o = (PediaWin *)(w != 0 ? w - 0x26 : w);
    switch (key) {
    case 0xd2:
        win_close_1060_2a47(o);
        break;
    default:
        if (o->modal == 0)
            keyboard_accelerator_handler(key);
        break;
    }
}
