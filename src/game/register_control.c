/* register_control -- shipped at 1060:9980.
 *
 * Ported by tools/port.py from matched/game/register_control.c.
 *
 * The file's own Ctl, Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void register_control(Win *w, Ctl *c)
{
    Ctl *p;

    p = (*((Ctl * *)((unsigned char *)(w) + 0x8c)));
    if (p == 0) {
        (*((Ctl * *)((unsigned char *)(w) + 0x8c))) = c;
    } else {
        while ((*((void * *)((unsigned char *)(p) + 0x12))) != 0)
            p = (*((void * *)((unsigned char *)(p) + 0x12)));
        (*((void * *)((unsigned char *)(p) + 0x12))) = c;
    }
}
