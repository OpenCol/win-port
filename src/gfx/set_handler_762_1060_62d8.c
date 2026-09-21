/* set_handler_762_1060_62d8 -- shipped at 1060:62d8.
 *
 * Ported by tools/port.py from matched/gfx/set_handler_762_1060_62d8.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_handler_762_1060_62d8(Win *w, void *h)
{
    (*((void * *)((unsigned char *)(w) + 0x762))) = h;
}
