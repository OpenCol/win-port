/* ctl_copy_text -- shipped at 1050:0c96.
 *
 * Ported by tools/port.py from matched/game/ctl_copy_text.c.
 *
 * STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * heap_get(void *arena, col_i16 n);

void ctl_copy_text(Ctl *c, char *s)
{
    (*((char * *)((unsigned char *)(c) + 0x362))) = heap_get(((char *)((unsigned char *)(c) + 0x392)), strlen(s) + 1);
    strcpy((*((char * *)((unsigned char *)(c) + 0x362))), s);
}
