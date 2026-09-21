/* set_handler_762_1038_e10b -- shipped at 1038:e10b.
 *
 * Ported by tools/port.py from matched/game/set_handler_762_1038_e10b.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_handler_762_1038_e10b(Win *w, void *h)
{
    (*((void * *)((unsigned char *)(w) + 0x762))) = h;
}
