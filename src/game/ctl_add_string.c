/* ctl_add_string -- shipped at 1050:167f.
 *
 * Ported by tools/port.py from matched/game/ctl_add_string.c.
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

void ctl_add_string(Ctl *c, char *s)
{
    if ((*((col_i16 *)((unsigned char *)(c) + 0x82))) < 3) {
        ((char * *)((unsigned char *)(c) + 0x3b8))[(*((col_i16 *)((unsigned char *)(c) + 0x82)))] = heap_get(((char *)((unsigned char *)(c) + 0x392)), strlen(s) + 1);
        strcpy(((char * *)((unsigned char *)(c) + 0x3b8))[(*((col_i16 *)((unsigned char *)(c) + 0x82)))], s);
        (*((col_i16 *)((unsigned char *)(c) + 0x82)))++;
    }
}
