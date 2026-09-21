/* rect_height_at_8_1058_c24c -- shipped at 1058:c24c.
 *
 * Ported by tools/port.py from matched/game/rect_height_at_8_1058_c24c.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * rect_height_1058_bd4c(void *p);

/* same object: 4-byte `push cs; call ` */


void * rect_height_at_8_1058_c24c(Obj *o)
{
    return rect_height_1058_bd4c(((char *)((unsigned char *)(o) + 0x8)));
}
